/*
 * A164245  Number of binary strings of length n with equal numbers of 00101
 *          and 01010 substrings.
 * https://oeis.org/A164245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with equal numbers
 *            of substrings 00101 and 01010. This implementation uses dynamic
 *            programming over the 4‑bit memory of the last four bits of the
 *            string (state 0…15) and the difference between the two pattern
 *            counts. The difference Δ = (#00101) – (#0101...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Constants for the two patterns (5 bits). */
    const unsigned pat1 = 0b00101;   /* pattern 00101  */
    const unsigned pat2 = 0b01010;   /* pattern 01010  */

    /* Maximum possible difference magnitude. */
    const idx_t max_occ = n / 5;                 /* floor(n/5)  */
    const idx_t diff_size = 2 * max_occ + 1;     /* 0 … 2*max_occ  */
    const idx_t offset   = max_occ;              /* index of Δ = 0   */

    /* Allocate DP layers: 16 states × diff_size columns. */
    seq_t *dp_cur[16];
    seq_t *dp_next[16];
    for (idx_t s = 0; s < 16; ++s) {
        dp_cur[s]  = (seq_t*)calloc((size_t)diff_size, sizeof(seq_t));
        dp_next[s] = (seq_t*)calloc((size_t)diff_size, sizeof(seq_t));
    }

    /* Initial state: empty string, difference 0, state 0. */
    dp_cur[0][offset] = 1;

    /* Main DP loop over each bit position. */
    for (idx_t len = 0; len < n; ++len) {
        /* Reset next layer to zero. */
        for (idx_t s = 0; s < 16; ++s)
            memset(dp_next[s], 0, (size_t)diff_size * sizeof(seq_t));

        for (idx_t s = 0; s < 16; ++s) {
            seq_t *row_cur = dp_cur[s];
            for (idx_t d = 0; d < diff_size; ++d) {
                seq_t val = row_cur[d];
                if (val == 0) continue;        /* no strings reach this state */

                for (unsigned bit = 0; bit < 2; ++bit) {
                    const idx_t ns = ((s << 1) | bit) & 15; /* new 4‑bit state */
                    idx_t nd = d;                        /* new diff index */

                    if (len >= 4) {   /* a 5‑bit window has just ended */
                        const unsigned window = ((s << 1) | bit) & 31;
                        if (window == pat1) {
                            if (nd + 1 < diff_size) nd = nd + 1;
                        } else if (window == pat2) {
                            if (nd > 0) nd = nd - 1;
                        }
                    }

                    dp_next[ns][nd] += val;
                }
            }
        }

        /* Swap layers for the next iteration. */
        for (idx_t s = 0; s < 16; ++s) {
            seq_t *tmp     = dp_cur[s];
            dp_cur[s]      = dp_next[s];
            dp_next[s]     = tmp;
        }
    }

    /* Sum over all states with difference 0. */
    seq_t result = 0;
    for (idx_t s = 0; s < 16; ++s)
        result += dp_cur[s][offset];

    /* Free allocated memory. */
    for (idx_t s = 0; s < 16; ++s) {
        free(dp_cur[s]);
        free(dp_next[s]);
    }

    return result;
}
