/*
 * A179349  Number of permutations of 1..n with i-8<=p(i)<=i+2.
 * https://oeis.org/A179349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of permutations of 1..n with i-8 <= p(i) <= i+2
   using dynamic programming on a sliding window of size 11.  This is the
   permanent of the 0/1 band matrix with 1's in positions [i-8,i+2]. */
#include <stdlib.h>
#include <string.h>

#define BAND 11
#define FULL_MASK ((1u << BAND) - 1)

seq_t a(idx_t n)
{
    /* Allocate two DP tables of size 2^11 = 2048.  The type seq_t is an
       integer type (possibly arbitrary precision) provided by the build. */
    size_t states = 1u << BAND;
    seq_t *dp_curr = (seq_t *)calloc(states, sizeof(seq_t));
    seq_t *dp_next = (seq_t *)calloc(states, sizeof(seq_t));
    if (!dp_curr || !dp_next) return (seq_t)0;  /* out of memory (unlikely) */

    /* Initial mask for i = 1.  Bits correspond to numbers j = i-8 + k.  */
    unsigned int init_mask = 0;
    for (unsigned int k = 0; k < BAND; ++k) {
        idx_t j = (idx_t)1 - 8 + k;           /* j = 1-8 + k = k-7 */
        if (j < 1 || j > n) init_mask |= (1u << k);
    }
    dp_curr[init_mask] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        memset(dp_next, 0, states * sizeof(seq_t));

        for (unsigned int mask = 0; mask < states; ++mask) {
            seq_t cur = dp_curr[mask];
            if (cur == 0) continue;

            for (unsigned int k = 0; k < BAND; ++k) {
                idx_t j = i - 8 + k;           /* candidate number for position i */
                if (j < 1 || j > n) continue;  /* j out of global bounds */
                if (mask & (1u << k)) continue;/* already used in this window */

                unsigned int new_mask = mask | (1u << k);
                unsigned int nxt = new_mask >> 1;  /* shift window to next i */

                /* Add the new rightmost number (i+3).  It is used if it lies
                   outside 1..n.  */
                if (i + 3 > n)
                    nxt |= (1u << (BAND - 1));

                dp_next[nxt] += cur;
            }
        }

        /* Swap current and next tables for the next iteration. */
        seq_t *temp = dp_curr;
        dp_curr = dp_next;
        dp_next = temp;
    }

    seq_t ans = dp_curr[FULL_MASK];

    free(dp_curr);
    free(dp_next);
    return ans;
}
