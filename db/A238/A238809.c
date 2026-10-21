/*
 * A238809  Number of n X 5 0..2 arrays with no element equal to one plus the
 *          sum of elements to its left or one plus the sum of the sum of
 *          elements above it, modulo 3.
 * https://oeis.org/A238809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 arrays with entries {0,1,2} such that * for
 *            every cell (i,j) the value is NOT congruent to * 1 + sum of
 *            entries to its left (in the same row) (mod 3) * 1 + sum of
 *            entries above it (in the same column) (mod 3). * Computed by
 *            dynamic programming over 3‑ary column‑sum states
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 19, u64 n <= 89, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* DP over states of column sums modulo 3.
 * Each state is encoded as a base‑3 integer 0‑242.
 */
static int pow3_global[5];
static unsigned char digits_global[243][5];
static unsigned char trans_count[243];
static int trans_next[243][243];
static bool initialized = false;

static void init_globals(void) {
    /* powers of 3 */
    pow3_global[0] = 1;
    for (int i = 1; i < 5; ++i) pow3_global[i] = pow3_global[i-1] * 3;

    /* pre‑compute digit representation of each state */
    for (int s = 0; s < 243; ++s) {
        int tmp = s;
        for (int i = 0; i < 5; ++i) {
            digits_global[s][i] = tmp % 3;
            tmp /= 3;
        }
    }

    /* pre‑compute transitions for each state */
    for (int s = 0; s < 243; ++s) {
        trans_count[s] = 0;
        const unsigned char *old = digits_global[s];
        for (int code = 0; code < 243; ++code) {
            unsigned char row[5];
            int tmp = code;
            for (int i = 0; i < 5; ++i) {
                row[i] = tmp % 3;
                tmp /= 3;
            }

            /* check constraints */
            bool ok = true;
            int left_sum = 0;
            for (int j = 0; j < 5; ++j) {
                int val = row[j];
                if (val == (1 + old[j]) % 3) { ok = false; break; }
                if (val == (1 + left_sum) % 3) { ok = false; break; }
                left_sum = (left_sum + val) % 3;
            }
            if (!ok) continue;

            /* compute new state */
            int new_state = 0;
            for (int j = 0; j < 5; ++j) {
                new_state += ((old[j] + row[j]) % 3) * pow3_global[j];
            }
            trans_next[s][trans_count[s]++] = new_state;
        }
    }

    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized) init_globals();

    /* DP arrays */
    seq_t dp[243];
    seq_t next[243];
    for (int i = 0; i < 243; ++i) dp[i] = 0;
    dp[0] = (seq_t)1;          /* start from zero column sums */

    for (idx_t r = 0; r < n; ++r) {
        for (int i = 0; i < 243; ++i) next[i] = 0;
        for (int s = 0; s < 243; ++s) {
            seq_t cur = dp[s];
            if (cur == (seq_t)0) continue;
            int cnt = trans_count[s];
            for (int t = 0; t < cnt; ++t) {
                int ns = trans_next[s][t];
                next[ns] = next[ns] + cur;
            }
        }
        for (int i = 0; i < 243; ++i) dp[i] = next[i];
    }

    seq_t total = (seq_t)0;
    for (int i = 0; i < 243; ++i) total += dp[i];
    return total;
}
