/*
 * A129343  a(2n) = a(n), a(2n+1) = 4n+1.
 * https://oeis.org/A129343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; for n > 0, let m = n while divisible by 2, m = n >>
 *            v2(n) (the odd part of n); then a(n) = 2 * m - 1. Equivalent to
 *            a(2n)=a(n), a(2n+1)=4n+1
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;
    /* Remove all factors of 2; n becomes odd */
    while ((n & 1u) == 0u)
        n >>= 1u;
    /* Now n is odd and > 0. Compute 2*n - 1. */
    return ((seq_t)n << 1) - (seq_t)1;
}
