/*
 * A212229  Number of 0..2 arrays of length n+9 with sum no more than 10 in
 *          any length 10 subsequence (=50% duty cycle).
 * https://oeis.org/A212229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 12, u64 n <= 35, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of 0..2 arrays of length n+9 with sum <=10 in every
   length‑10 contiguous subarray (OEIS A212229).   */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define N_STATES 19683   /* 3^9 */

seq_t a(idx_t n)
{
    /* Stateless static tables and arrays, initialized on first call. */
    static bool tables_inited = false;
    static idx_t sum_state[N_STATES];
    static int next_state[N_STATES][3];
    static seq_t *cur_counts = NULL;
    static seq_t *next_counts = NULL;
    static idx_t processed = 0;   /* number of steps already computed (i.e. max n processed) */

    /* Initialise state tables and count arrays */
    if (!tables_inited) {
        /* pre‑compute powers of 3 up to 3^8 */
        idx_t pow3[10];
        pow3[0] = 1;
        for (idx_t i = 1; i <= 9; ++i) pow3[i] = pow3[i-1] * 3;

        /* For each state (9‑digit base‑3 number) compute digit sum
           and transition to the state after appending 0,1,2. */
        for (idx_t s = 0; s < N_STATES; ++s) {
            /* digit sum */
            int tmp = (int)s;
            int ss = 0;
            for (int d = 0; d < 9; ++d) {
                ss += tmp % 3;
                tmp /= 3;
            }
            sum_state[s] = (idx_t)ss;

            /* transitions */
            idx_t lower8 = s % pow3[8];   /* keep last 8 digits */
            for (int x = 0; x < 3; ++x) {
                next_state[s][x] = (int)(lower8 * 3 + x);
            }
        }

        /* Allocate DP arrays */
        cur_counts  = (seq_t*)malloc(N_STATES * sizeof(seq_t));
        next_counts = (seq_t*)malloc(N_STATES * sizeof(seq_t));

        /* initial counts: for length 9 all states are possible (no window constraint yet) */
        for (idx_t s = 0; s < N_STATES; ++s) {
            cur_counts[s] = 1;
            next_counts[s] = 0;
        }

        tables_inited = true;
    }

    /* Extend DP to the required number of extra steps (n). */
    if (n > processed) {
        idx_t steps = n - processed;
        for (idx_t step = 0; step < steps; ++step) {
            /* zero out next array */
            for (idx_t s = 0; s < N_STATES; ++s) next_counts[s] = 0;

            /* transition */
            for (idx_t s = 0; s < N_STATES; ++s) {
                seq_t cnt = cur_counts[s];
                if (!cnt) continue;
                idx_t ss = sum_state[s];
                for (int x = 0; x < 3; ++x) {
                    if (ss + x <= 10) {
                        idx_t ns = next_state[s][x];
                        next_counts[ns] += cnt;
                    }
                }
            }

            /* swap arrays for next iteration */
            seq_t *tmp = cur_counts;
            cur_counts = next_counts;
            next_counts = tmp;
        }
        processed = n;
    }

    /* Sum all counts to obtain the total number of valid arrays of
       length n+9.  cur_counts now holds the state distribution for
       that length. */
    seq_t total = 0;
    for (idx_t s = 0; s < N_STATES; ++s) {
        total += cur_counts[s];
    }
    return total;
}
