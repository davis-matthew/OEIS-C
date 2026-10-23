/*
 * A107585  a(n) = 5^n - 5*n.
 * https://oeis.org/A107585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 5*n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast exponentiation: compute 5^n */
    seq_t result = 1;        /* 5^0 = 1 */
    seq_t base   = 5;        /* 5^1 = 5 */
    idx_t  exp   = n;        /* copy of n for shifting */

    while (exp != 0) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    return result - (seq_t)n * (seq_t)5;
}
