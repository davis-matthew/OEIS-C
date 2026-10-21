/*
 * A233158  Number of 5Xn 0..2 arrays with no element x(i,j) adjacent to value
 *          2-x(i,j) horizontally or antidiagonally.
 * https://oeis.org/A233158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence a(n) = 16*a(n-1) -64*a(n-2) +76*a(n-3) +17*a(n-4)
 *            -84*a(n-5) +48*a(n-6) -8*a(n-7) for n>11, with a(1)=243,
 *            a(2)=1536, a(3)=15552, a(4)=157464, a(5)=1658232, a(6)=17587584,
 *            a(7)=187446312, a(8)=2000708640. The implementation below uses
 *            dynamic programming over the 243 possible column st...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Number of possible column states (3^5). */
#define STATES 243
/* Number of rows. */
#define ROWS 5
/* Maximum number of predecessors a state can have. */
#define MAX_PREV STATES
/* Maximum n we support (sufficient for the contest limits). */
#define MAX_ANS 32769

/* Precomputed base‑3 digits of each state: state_val[s][r] gives the value
   (0,1,2) in row r of column state s (row 0 = least significant digit). */
static unsigned char state_val[STATES][ROWS];
/* For each state s: prev_start[s] marks the start index in prev_list for
   predecessors of s; prev_start[STATES] is the total number of pairs. */
static size_t prev_start[STATES + 1];
/* Flat list of predecessor indices. */
static unsigned char prev_list[STATES * MAX_PREV];

/* DP arrays for the previous and current column. */
static seq_t dpPrev[STATES];
static seq_t dpCurr[STATES];

/* Static cache of answers for n = 1..MAX_ANS-1. */
static seq_t answers[MAX_ANS];
/* Largest n for which answers[] holds a valid value. */
static idx_t computed_n = 0;

/* ------------------------------------------------------------------ */
/* Fill state_val with base‑3 digits. */
static void init_state_values(void)
{
    for (int s = 0; s < STATES; ++s) {
        int x = s;
        for (int r = 0; r < ROWS; ++r) {
            state_val[s][r] = (unsigned char)(x % 3);
            x /= 3;
        }
    }
}

/* Test whether transition from state p to state s is allowed. */
static inline bool allowed(int p, int s)
{
    for (int r = 0; r < ROWS; ++r) {
        if (state_val[p][r] + state_val[s][r] == 2)
            return false;                 /* horizontal adjacency forbidden */
        if (r < ROWS - 1 &&
            state_val[p][r] + state_val[s][r + 1] == 2)
            return false;                 /* antidiagonal adjacency forbidden */
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Initialise static tables once. */
static void init_tables(void)
{
    static bool done = false;
    if (done) return;

    init_state_values();

    /* Count predecessors for each state. */
    for (int s = 0; s <= STATES; ++s)
        prev_start[s] = 0;
    for (int p = 0; p < STATES; ++p) {
        for (int s = 0; s < STATES; ++s) {
            if (allowed(p, s))
                prev_start[s + 1]++;      /* accumulate count for s */
        }
    }
    /* Cumulative sums. */
    for (int s = 0; s < STATES; ++s)
        prev_start[s + 1] += prev_start[s];

    /* Temporary copy for filling. */
    size_t tmp_start[STATES + 1];
    for (int s = 0; s <= STATES; ++s)
        tmp_start[s] = prev_start[s];

    /* Build predecessor list. */
    for (int p = 0; p < STATES; ++p) {
        for (int s = 0; s < STATES; ++s) {
            if (allowed(p, s))
                prev_list[tmp_start[s]++] = (unsigned char)p;
        }
    }

    done = true;
}

/* ------------------------------------------------------------------ */
/* Compute answers[1..target_n] if not already done. */
static void compute_to(idx_t target_n)
{
    init_tables();
    if (computed_n >= target_n)
        return;

    if (target_n >= MAX_ANS) {
        /* Out of bounds; can't compute.  Abort. */
        abort();
    }

    /* If nothing computed yet, initialise for n=1. */
    if (computed_n == 0) {
        for (int s = 0; s < STATES; ++s)
            dpPrev[s] = (seq_t)1;
        seq_t sum = 0;
        for (int s = 0; s < STATES; ++s)
            sum += dpPrev[s];
        answers[1] = sum;
        computed_n = 1;
    }

    /* Extend DP up to target_n. */
    for (idx_t k = computed_n + 1; k <= target_n; ++k) {
        for (int s = 0; s < STATES; ++s) {
            seq_t sum = 0;
            size_t start = prev_start[s];
            size_t end   = prev_start[s + 1];
            for (size_t i = start; i < end; ++i) {
                unsigned char p = prev_list[i];
                sum += dpPrev[p];
            }
            dpCurr[s] = sum;
        }
        seq_t total = 0;
        for (int s = 0; s < STATES; ++s)
            total += dpCurr[s];
        answers[k] = total;

        /* Prepare for next iteration. */
        for (int s = 0; s < STATES; ++s)
            dpPrev[s] = dpCurr[s];
    }

    computed_n = target_n;
}

/* ------------------------------------------------------------------ */
/* Public interface required by the platform. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    compute_to(n);
    return answers[n];
}
