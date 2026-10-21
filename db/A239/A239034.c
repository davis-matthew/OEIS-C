/*
 * A239034  Number of 7Xn 0..2 arrays with no element equal to the sum of
 *          elements to its left or one plus the sum of the elements above it,
 *          modulo 3.
 * https://oeis.org/A239034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of 7×n arrays over {0,1,2} where no element equals
 *            * (sum of elements to its left) mod 3 or (1 + sum of elements
 *            above it) mod 3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 131, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define STATE_SIZE 2187   /* 3^7 */

static int digits[STATE_SIZE][7];
static int  *t_next[STATE_SIZE];   /* next state indices                */
static int  *t_count[STATE_SIZE];  /* corresponding counts (0‑based)    */
static size_t t_len[STATE_SIZE];   /* number of transitions per state   */
static int   transitions_ready = 0;

/* Pre‑compute the transition lists for all 3^7 states. */
static void init_transitions(void)
{
    if (transitions_ready) return;
    transitions_ready = 1;

    /* Convert each integer 0..2186 to its base‑3 digit representation. */
    for (int i = 0; i < STATE_SIZE; ++i) {
        int v = i;
        for (int r = 6; r >= 0; --r) {   /* r = row index 0…6 */
            digits[i][r] = v % 3;
            v /= 3;
        }
    }

    /* For every state, enumerate all 3^7 possible column assignments,
     * test the restrictions, and count resulting transitions. */
    for (int s = 0; s < STATE_SIZE; ++s) {
        int tmp[STATE_SIZE];
        for (int k = 0; k < STATE_SIZE; ++k) tmp[k] = 0;

        for (int a = 0; a < STATE_SIZE; ++a) {
            int ok = 1;
            int above = 0;
            for (int r = 0; r < 7; ++r) {
                int need = digits[s][r];
                int val  = digits[a][r];
                if (val == need || val == (1 + above) % 3) { ok = 0; break; }
                above = (above + val) % 3;
            }
            if (!ok) continue;

            int next = 0;
            for (int r = 0; r < 7; ++r) {
                int val = (digits[s][r] + digits[a][r]) % 3;
                next = next * 3 + val;
            }
            ++tmp[next];
        }

        /* Count non‑zero transitions. */
        size_t len = 0;
        for (int t = 0; t < STATE_SIZE; ++t)
            if (tmp[t]) ++len;

        t_len[s] = len;
        if (len == 0) {
            t_next[s] = NULL;
            t_count[s] = NULL;
            continue;
        }

        t_next[s]  = (int*)malloc(len * sizeof(int));
        t_count[s] = (int*)malloc(len * sizeof(int));
        if (!t_next[s] || !t_count[s]) {
            /* Allocation failed – abort. */
            exit(1);
        }

        size_t idx = 0;
        for (int t = 0; t < STATE_SIZE; ++t) {
            if (tmp[t]) {
                t_next[s][idx]  = t;
                t_count[s][idx] = tmp[t];
                ++idx;
            }
        }
    }
}

seq_t a(idx_t n)
{
    init_transitions();

    if (n == 0) return 0;

    static seq_t dp_cur[STATE_SIZE];
    static seq_t dp_next[STATE_SIZE];

    /* initialise dp array */
    for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = 0;
    dp_cur[0] = (seq_t)1;

    /* dynamic programming over n columns */
    for (idx_t col = 1; col <= n; ++col) {
        for (int i = 0; i < STATE_SIZE; ++i) dp_next[i] = 0;

        for (int s = 0; s < STATE_SIZE; ++s) {
            if (dp_cur[s] == 0) continue;
            size_t len = t_len[s];
            for (size_t k = 0; k < len; ++k) {
                int t    = t_next[s][k];
                seq_t prod = dp_cur[s] * (seq_t)t_count[s][k];
                dp_next[t] += prod;
            }
        }

        /* copy next into current for the next iteration */
        for (int i = 0; i < STATE_SIZE; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum over all possible ending states */
    seq_t result = 0;
    for (int i = 0; i < STATE_SIZE; ++i) result += dp_cur[i];
    return result;
}
