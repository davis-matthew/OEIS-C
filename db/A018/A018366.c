/*
 * A018366  Divisors of 272.
 * https://oeis.org/A018366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = 1 << (n-1) for 1 ≤ n ≤ 5 a(n) = 17 << (n-6) for 6 ≤ n ≤
 *            10
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for n = 0, but return 0 */
    if (n <= 5)
        return (seq_t)1 << (unsigned int)(n - 1);
    return (seq_t)17 << (unsigned int)(n - 6);
}
