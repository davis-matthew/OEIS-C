/*
 * A189584  Number of permutations of 1..n with displacements restricted to
 *          {-4,0,1,2,3}.
 * https://oeis.org/A189584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A189584: Count permutations of 1..n with displacements
   restricted to the set { -4, 0, 1, 2, 3 }.  The algorithm
   uses dynamic programming with an 8‑bit sliding mask that
   represents which of the numbers in the window [i‑4, i+3]
   have already been used when the algorithm is at position i. */
#include <stddef.h>
#include <stdbool.h>

#define MASK_SIZE 256   /* 2^8, all possible masks */

seq_t a(idx_t n)
{
    seq_t prev[MASK_SIZE];
    seq_t curr[MASK_SIZE];
    /* clear arrays */
    for (int m = 0; m < MASK_SIZE; ++m) prev[m] = 0;
    for (int m = 0; m < MASK_SIZE; ++m) curr[m] = 0;

    /* initial mask for position i = 1 */
    int init_mask = 0;
    for (int k = 0; k < 8; ++k) {
        /* value that bit k would represent at i = 1 */
        idx_t val = (idx_t)(1) - 4 + k;   /* = k-3 */
        if (val < 1 || val > n) init_mask |= 1 << k;
    }
    prev[init_mask] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* clear next DP layer */
        for (int m = 0; m < MASK_SIZE; ++m) curr[m] = 0;

        for (int m = 0; m < MASK_SIZE; ++m) {
            seq_t ways = prev[m];
            if (!ways) continue;

            /* displacements to try: -4, 0, 1, 2, 3 */
            const int disp[5] = { -4, 0, 1, 2, 3 };
            for (int di = 0; di < 5; ++di) {
                int d = disp[di];
                idx_t val = i + (idx_t)d;
                if (val < 1 || val > n) continue;
                int k = d + 4;   /* bit index for this displacement */

                if ((m >> k) & 1) continue;   /* already used */

                int m2 = m | (1 << k);        /* mark chosen number as used */

                int new_mask = (m2 >> 1);     /* shift window right */

                /* new number that enters the window at the right end */
                idx_t new_val = i + 4;
                if (new_val > n) {
                    new_mask |= 1 << 7;       /* mark as used if out of range */
                }
                /* else leave bit 7 as 0 for the new available value */

                curr[new_mask] += ways;
            }
        }
        /* advance to next position */
        for (int m = 0; m < MASK_SIZE; ++m) prev[m] = curr[m];
    }

    /* sum all final states – each represents one complete permutation */
    seq_t result = 0;
    for (int m = 0; m < MASK_SIZE; ++m) result += prev[m];
    return result;
}
