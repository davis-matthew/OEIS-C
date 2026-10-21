/*
 * A072854  Number of permutations satisfying i-3<=p(i)<=i+4, i=1..n.
 * https://oeis.org/A072854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of permutations of {1..n} where p(i) ∈ [i‑3, i+4]
 *            for each i. The implementation uses a sliding‑window dynamic
 *            programming with an 8‑bit mask representing the elements that
 *            are still unused in the current admissible window. For position
 *            i the window is [i‑3, i+4]; after choosing an...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Width of the admissible window is always 8 (i‑3 … i+4). */
    const int WIDTH = 8;
    const int ALL   = (1 << WIDTH) - 1;     /* 0xFF for WIDTH = 8 */

    /* DP tables – size 256 is a compile‑time constant. */
    seq_t dp[1 << WIDTH];
    seq_t next[1 << WIDTH];

    /* initialise with zeroes. */
    for (int i = 0; i <= ALL; ++i) {
        dp[i]   = 0;
        next[i] = 0;
    }

    /* Initial mask for position i = 1.
       Window for i=1 is [‑2, 5]; only values 1…min(5,n) are present. */
    int init_mask = 0;
    for (int b = 0; b < WIDTH; ++b) {
        int val = -2 + b;               /* value corresponding to bit b */
        if (val >= 1 && (idx_t)val <= n)
            init_mask |= 1 << b;
    }
    dp[init_mask] = 1;

    /* Process positions 1 … n. */
    for (idx_t i = 1; i <= n; ++i) {
        /* New element that will enter the window after this step. */
        int add_bit = (i + 5 <= n) ? (1 << (WIDTH - 1)) : 0;   /* bit 7 */

        /* clear next table */
        for (int m = 0; m <= ALL; ++m) next[m] = 0;

        /* transition */
        for (int mask = 0; mask <= ALL; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;

            for (int b = 0; b < WIDTH; ++b) {
                if (mask & (1 << b)) {
                    /* use element represented by bit b */
                    int new_mask = ((mask & ~(1 << b)) >> 1) | add_bit;
                    next[new_mask] += ways;
                }
            }
        }

        /* move to next state */
        for (int m = 0; m <= ALL; ++m) dp[m] = next[m];
    }

    /* All elements must have been used – mask must be zero. */
    return dp[0];
}
