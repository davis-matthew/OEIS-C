/*
 * A179341  Number of permutations of 1..n with i-6<=p(i)<=i+3.
 * https://oeis.org/A179341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of permutations of 1..n with i-6 <= p(i) <= i+3.
   The computation uses a bitmask DP over a 10‑position window (positions
   i-6 .. i+3).  For each i the current mask indicates which of the
   up to ten possible images are still unused.  At step i a choice
   removes one bit, the mask is shifted right (since i increases),
   and a new bit for the new right‑most position (j = i+4) is added
   if it lies within [1..n].  The answer is the number of ways to
   finish with an empty mask.  Complexity: O(n·2¹⁰).  */

#include <stdbool.h>

static const int WIDTH  = 10;          /* offsets -6 .. +3 inclusive */
static const int MASKSIZE = 1 << WIDTH;

seq_t a(idx_t n)
{
    /* Current and next DP tables */
    seq_t cur[MASKSIZE];
    seq_t next_arr[MASKSIZE];

    /* Zero‑initialize the tables */
    for (int i = 0; i < MASKSIZE; ++i) {
        cur[i]     = 0;
        next_arr[i] = 0;
    }

    /* Initial mask for i = 1:
       only right positions j ∈ [max(1,1-6) , min(n,1+3)] are free */
    int initMask = 0;
    int jmax = n < 4 ? (int)n : 4;          /* j ≤ min(n,4) */
    for (int j = 1; j <= jmax; ++j) {
        int idx = j - 1 + 6;                /* offset = j-1 → idx = offset+6 */
        initMask |= 1 << idx;
    }
    cur[initMask] = 1;

    for (idx_t i = 1; i <= n; ++i) {
        /* Clear next_arr */
        for (int k = 0; k < MASKSIZE; ++k) next_arr[k] = 0;

        for (int mask = 0; mask < MASKSIZE; ++mask) {
            seq_t cnt = cur[mask];
            if (!cnt) continue;

            unsigned int avail = (unsigned int)mask;
            while (avail) {
                int k = __builtin_ctz(avail);          /* lowest set bit */
                avail &= avail - 1;                   /* clear that bit */

                /* Remove chosen j, then shift for next i */
                int newMask = (mask & ~(1 << k)) >> 1;

                /* Add new rightmost possible j (j = i+4) if it lies within bounds */
                if ((i + 4) <= n)
                    newMask |= 1 << 9;                 /* offset +3 → idx 9 */

                next_arr[newMask] += cnt;
            }
        }

        /* Move to next stage */
        for (int k = 0; k < MASKSIZE; ++k) cur[k] = next_arr[k];
    }

    return cur[0];
}
