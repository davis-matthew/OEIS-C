/*
 * A065140  a(n) = 2^n*(2*n)!.
 * https://oeis.org/A065140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (2n)!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1. Compute (2n)! */
    seq_t res = 1;
    idx_t limit = n << 1;          /* 2 * n */
    for (idx_t i = 1; i <= limit; ++i) {
        res *= (seq_t)i;           /* multiply by current i */
    }

    /* 2. Multiply by 2^n  */
    res <<= n;                     /* shift left by n bits (×2^n) */

    return res;
}
