/*
 * A133751  a(n) = 2*(2+n)! + 2^n.
 * https://oeis.org/A133751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2 + n)! + 2^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute factorial (n+2)! */
    seq_t fact = 1;
    idx_t m = n + 2;
    for (idx_t i = 2; i <= m; ++i) {
        fact *= (seq_t)i;
    }
    /* 2*(n+2)! */
    fact *= 2;

    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= 2;
    }

    return fact + pow2;
}
