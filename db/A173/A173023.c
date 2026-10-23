/*
 * A173023  Number of numbers <= n whose binary representation contains no
 *          isolated digits "11".
 * https://oeis.org/A173023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = # of integers k (0 ≤ k ≤ n) whose binary representation
 *            contains no isolated run of exactly two consecutive 1's (i.e.,
 *            no substring 11 that is not part of a longer run)
 * Range:     u8 n <= 512, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#ifndef SEQ_T_DEFINED
/* seq_t and idx_t are defined by the build system.  No typedefs needed here. */
#endif

static seq_t dp_table[65][4];
static bool dp_prepared = false;

/* Compute dp_table[len][run] = number of bit strings of length len
   that can follow a prefix ending with a run of 1's of length 'run'
   (run = 0,1,2,3 where 3 represents a run of length >=3 or no run). */
static void prepare_dp(void)
{
    if (dp_prepared) return;
    for (int run = 0; run < 4; ++run)
        dp_table[0][run] = (run == 2) ? (seq_t)0 : (seq_t)1;

    for (int len = 1; len <= 64; ++len) {
        for (int run = 0; run < 4; ++run) {
            seq_t sum = 0;
            /* bit 0 */
            if (run != 2) {
                sum += dp_table[len - 1][0];
            }
            /* bit 1 */
            int new_run;
            if (run == 0)          new_run = 1;
            else if (run == 1)     new_run = 2;
            else /* run==2 or 3 */ new_run = 3;
            sum += dp_table[len - 1][new_run];
            dp_table[len][run] = sum;
        }
    }
    dp_prepared = true;
}

/* Return a(n) as defined above. */
seq_t a(idx_t n)
{
    prepare_dp();

    /* Extract bits of n, msb first. */
    int bits[64];
    int len = 0;
    idx_t temp = n;
    if (temp == 0) {
        bits[0] = 0;
        len = 1;
    } else {
        while (temp != 0) {
            bits[len++] = 1 & temp;
            temp >>= 1;
        }
        /* reverse to msb first */
        for (int i = 0; i < len / 2; ++i) {
            int t = bits[i];
            bits[i] = bits[len - 1 - i];
            bits[len - 1 - i] = t;
        }
    }

    seq_t ans = 0;
    int run_len = 0;               /* current run length of consecutive 1's seen so far */
    bool tight = true;             /* whether prefix matches n exactly so far */

    for (int pos = 0; pos < len; ++pos) {
        int n_bit = bits[pos];
        int remaining = len - 1 - pos;

        /* Bits less than the current bit of n */
        for (int b = 0; b < n_bit; ++b) {
            int new_run = 0;
            bool ok = true;
            if (b == 0) {
                /* b=0 */
                if (run_len == 2) ok = false;
                else new_run = 0;
            } else { /* b==1 */
                if (run_len == 0)          new_run = 1;
                else if (run_len == 1)     new_run = 2;
                else                       new_run = 3;
            }
            if (!ok) continue;
            ans += dp_table[remaining][new_run];
        }

        /* Now follow the actual bit of n */
        int b = n_bit;
        int new_run = 0;
        bool ok = true;
        if (b == 0) {
            if (run_len == 2) ok = false;
            else new_run = 0;
        } else { /* b==1 */
            if (run_len == 0)          new_run = 1;
            else if (run_len == 1)     new_run = 2;
            else                       new_run = 3;
        }
        if (!ok) {
            /* The prefix already violates the rule, stop counting. */
            return ans;
        }
        run_len = new_run;
        /* tight remains true because we matched the bit */
    }

    /* After processing all bits, we need to include the number itself,
       if it doesn't end with an isolated run of two 1's. */
    if (run_len != 2) ans += 1;
    return ans;
}
