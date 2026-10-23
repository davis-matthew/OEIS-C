/*
 * A175885  Numbers that are congruent to {1, 10} mod 11.
 * https://oeis.org/A175885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (22*n + 7*(-1)^n - 11)/4
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (-1)^n is 1 for even n and -1 for odd n */
    int s = (n & 1) ? -1 : 1;
    long long tmp = 22LL * (long long)n + 7LL * (long long)s - 11LL;
    return (seq_t)(tmp / 4);
}
