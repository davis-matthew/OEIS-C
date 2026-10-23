/*
 * A153280  Eigensequence of triangle A153279.
 * https://oeis.org/A153280
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n+1) = a(n) * (2 + 3^n) for n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) = 1 */
    seq_t pow3 = (seq_t)1;            /* 3^0 = 1 */

    /* Iterate n steps: multiply by (2 + 3^k) each time, increasing k */
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t factor = (seq_t)2 + pow3;   /* 2 + 3^k */
        result *= factor;                /* a(k+1) = a(k) * factor */

        /* Prepare next power of 3 */
        pow3 *= (seq_t)3;                /* 3^(k+1) */
    }

    return result;
}
