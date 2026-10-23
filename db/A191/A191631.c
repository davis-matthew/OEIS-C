/*
 * A191631  a(n) = floor((2^n)/(2n-1)).
 * https://oeis.org/A191631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor(2^n / (2*n-1))
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 38, u64 n <= 71, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes the OEIS sequence A191631: a(n) = floor(2^n/(2*n-1)) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; n>=1 in practice */

    /* den = 2*n - 1 ; compute in seq_t to keep all arithmetic in seq_t */
    seq_t den = ((seq_t)2 * (seq_t)n) - 1;

    seq_t rem = 0;          /* remainder in the long‑division */
    seq_t q   = 0;          /* quotient (the desired result)     */
    seq_t bit = 1;          /* the single 1 bit at the top of 2^n */

    /* Long division of power of two by den.
       We process bits from the most significant (bit n) down to 0. */
    for (idx_t i = n; ; --i) {
        /* bring in the next bit of the numerator (1 only for i==n) */
        rem = (rem << 1) | bit;
        if (rem >= den) {
            rem -= den;
            q = (q << 1) | 1;
        } else {
            q <<= 1;
        }
        bit = 0;                       /* only the first iteration uses the high 1 bit */
        if (i == 0) break;             /* reached the least significant bit */
    }

    return q;
}
