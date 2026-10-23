/*
 * A189586  Number of permutations of 1..n with displacements restricted to
 *          {-4,-2,-1,0,3}.
 * https://oeis.org/A189586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 61, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Number of permutations of 1..n with displacements restricted to {-4,-2,-1,0,3}.
   The algorithm uses a dynamic programming over a sliding window of 8 possible values
   relative to the current position.  At each step the 8‑bit mask indicates which of
   the values [i-4 .. i+3] have already been matched.  The mask is shifted right
   after each position, dropping the leftmost value (which must already be matched),
   and a new bit is inserted for the value i+4.  This yields an O(n·256) solution
   that works for arbitrary n and any integer type assigned to seq_t. */

#include <stdbool.h>
#include <stdint.h>

static const int DISP[5] = {-4, -2, -1, 0, +3};

seq_t a(idx_t n)
{
    seq_t cur[256] = { 0 };
    seq_t nxt[256] = { 0 };
    int init_mask = 0;

    /* Build initial mask for i = 1.
       Bits 0..7 correspond to values [ -3 .. 4 ] (i-4 .. i+3).
       Any value outside [1..n] is marked as already used (bit = 1). */
    for (int k = 0; k < 8; ++k) {
        idx_t val = (idx_t)1 - 4 + k;   /* value = i-4 + k, with i = 1 */
        if (val < 1 || val > n)
            init_mask |= (1 << k);
    }
    cur[init_mask] = 1;

    /* Main DP loop over positions i = 1 .. n */
    for (idx_t i = 1; i <= n; ++i) {
        /* Reset next array */
        for (int s = 0; s < 256; ++s) nxt[s] = 0;

        for (int m = 0; m < 256; ++m) {
            seq_t ways = cur[m];
            if (!ways) continue;

            for (int d = 0; d < 5; ++d) {
                idx_t j = i + DISP[d];
                if (j < 1 || j > n) continue;

                int k = (int)(j - (i - 4));   /* bit index 0..7 */
                if (m & (1 << k)) continue;   /* already used */

                int new_mask = m | (1 << k);

                /* The leftmost value i-4 must be matched before it leaves the window */
                if ((new_mask & 1) == 0) continue;

                int shifted = (new_mask >> 1);
                if (i + 4 > n) shifted |= 0x80;  /* set new rightmost bit */

                nxt[shifted] += ways;
            }
        }
        /* Swap cur and nxt */
        for (int s = 0; s < 256; ++s) cur[s] = nxt[s];
    }

    /* After all positions processed, all values must be matched:
       mask == 0xFF (all bits set). */
    return cur[0xFF];
}
