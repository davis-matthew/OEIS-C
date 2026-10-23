/*
 * A178147  Sum of squares d^2 of distinct divisors of n, d in {2, 3, 5}.
 * https://oeis.org/A178147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = sum_{p in {2,3,5}, p|n} p^2
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n % 2 == 0) res += (seq_t)4;    /* 2^2 = 4 */
    if (n % 3 == 0) res += (seq_t)9;    /* 3^2 = 9 */
    if (n % 5 == 0) res += (seq_t)25;   /* 5^2 = 25 */
    return res;
}
