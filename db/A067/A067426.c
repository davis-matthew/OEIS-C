/*
 * A067426  Sixth column of triangle A067425.
 * https://oeis.org/A067426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 9 * 24^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t result = (seq_t)9;          /* 9 * 24^0 */
    idx_t  exp    = n - 1;           /* exponent of 24 */
    seq_t  base   = (seq_t)24;        /* 24^1 */

    /* fast exponentiation by squaring */
    while (exp) {
        if (exp & 1ULL) result = result * base;
        base = base * base;
        exp >>= 1ULL;
    }
    return result;
}
