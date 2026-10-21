/*
 * A205833  Number of (n+1)X6 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two.
 * https://oeis.org/A205833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Transfer‑matrix DP for A205833.
    a(n) = number of (n+1)×6 matrices with entries 0..2 such that every 2×2
    block has exactly two clockwise edge increases.  The algorithm builds
    the 3^6 = 729 possible row vectors, precomputes which pairs of rows
    are compatible, and then walks that directed graph for n steps.
    The sequence values are memoised.  The implementation uses static
    storage so it works for both C11 and C++17 and for the arbitrary‑precision
    build.                                                    */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define STATES 729                /* 3^6 */
#define MAX_N 20000              /* sufficient for the required limits */
#define MAX_TRANS 600000         /* upper bound on number of edges */

/* --- transition data ------------------------------------ */
static int state_row[STATES][6];
static unsigned short next_len[STATES];
static int* next_ptr[STATES];
static int  all_next[MAX_TRANS];
static int  trans_used = 0;

/* --- DP buffers ------------------------------------------ */
static seq_t dp0[STATES];
static seq_t dp1[STATES];
static seq_t *dp_curr = dp0;
static seq_t *dp_next = dp1;

/* --- memoisation ------------------------------------------ */
static seq_t cache[MAX_N + 1];
static size_t max_computed = 0;

/* --- initialisation ---------------------------------------- */
static void init_precomputation(void)
{
    /* generate all 3^6 rows */
    for (int s = 0; s < STATES; ++s) {
        int x = s;
        for (int i = 0; i < 6; ++i) {
            state_row[s][i] = x % 3;
            x /= 3;
        }
    }

    /* count and store neighbours */
    for (int a = 0; a < STATES; ++a) {
        int* ra = state_row[a];
        unsigned short cnt = 0;
        for (int b = 0; b < STATES; ++b) {
            int* rb = state_row[b];
            bool ok = true;
            for (int i = 0; i < 5 && ok; ++i) {
                int tl = ra[i],   tr = ra[i+1];
                int bl = rb[i],   br = rb[i+1];
                int inc = 0;
                if (tl < tr) ++inc;
                if (tr < br) ++inc;
                if (br < bl) ++inc;
                if (bl < tl) ++inc;
                if (inc != 2) ok = false;
            }
            if (ok) {
                all_next[trans_used++] = b;
                ++cnt;
            }
        }
        next_len[a] = cnt;
        next_ptr[a] = &all_next[trans_used - cnt];
    }

    /* initialise dp_curr: one way to start at each state */
    for (int s = 0; s < STATES; ++s) dp_curr[s] = (seq_t)1;
    /* dp_next already zeroed by static init */

    /* zero-cache (cache[0] unused) */
    for (size_t i = 0; i <= MAX_N; ++i) cache[i] = (seq_t)0;
    max_computed = 0;
}

/* --- sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined below offset */

    static bool initialized = false;
    if (!initialized) {
        init_precomputation();
        initialized = true;
    }

    /* extend memoised values up to n */
    while (max_computed < (size_t)n) {
        /* clear dp_next */
        for (int s = 0; s < STATES; ++s) dp_next[s] = (seq_t)0;

        /* one step transition */
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_curr[s];
            if (val == (seq_t)0) continue;
            unsigned short len = next_len[s];
            int* list = next_ptr[s];
            for (unsigned short i = 0; i < len; ++i) {
                dp_next[list[i]] += val;
            }
        }

        /* sum total for arrays of current length */
        seq_t total = (seq_t)0;
        for (int s = 0; s < STATES; ++s) total += dp_next[s];

        /* store result */
        if (max_computed + 1 <= MAX_N)
            cache[max_computed + 1] = total;
        ++max_computed;

        /* swap buffers for next iteration */
        seq_t *tmp = dp_curr;
        dp_curr = dp_next;
        dp_next = tmp;
    }

    return cache[n];
}
