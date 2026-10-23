/*
 * A189602  Number of permutations of 1..n with displacements restricted to
 *          {-7,-6,-5,-4,-3,0,1,2}.
 * https://oeis.org/A189602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n with displacements
 *            restricted to {-7,-6,-5,-4,-3,0,1,2}
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 52, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* sequence is indexed from 1 */

    /* Size of the sliding window: offsets -7 .. +2 (10 bits) */
    const int MASK_BITS = 10;
    const int MASK_SIZE = 1 << MASK_BITS;   /* 1024 */

    seq_t cur[MASK_SIZE];
    seq_t nxt[MASK_SIZE];

    /* Initial state: columns < 1 are already occupied (bits -7 .. -1) */
    int init_mask = (1 << 7) - 1;          /* bits 0..6 set to 1 */
    for (int i = 0; i < MASK_SIZE; ++i) cur[i] = 0;
    cur[init_mask] = 1;

    /* Allowed displacements */
    const int offs[8] = { -7, -6, -5, -4, -3, 0, 1, 2 };

    for (idx_t row = 1; row <= n; ++row) {
        for (int i = 0; i < MASK_SIZE; ++i) nxt[i] = 0;

        for (int mask = 0; mask < MASK_SIZE; ++mask) {
            seq_t val = cur[mask];
            if (val == 0) continue;

            for (int k = 0; k < 8; ++k) {
                int d = offs[k];
                int64_t jj = (int64_t)row + d;   /* avoid unsigned underflow */
                if (jj < 1 || jj > n) continue;

                int pos = d + 7;                 /* bit position for offset d */
                if (mask & (1u << pos)) continue; /* column already used */

                int new_mask = mask | (1u << pos);
                int shifted = new_mask >> 1;      /* advance to next row */
                nxt[shifted] += val;
            }
        }

        /* swap cur and nxt for next iteration */
        for (int i = 0; i < MASK_SIZE; ++i)
            cur[i] = nxt[i];
    }

    /* sum all states to get the total number of permutations */
    seq_t result = 0;
    for (int mask = 0; mask < MASK_SIZE; ++mask)
        result += cur[mask];

    return result;
}
