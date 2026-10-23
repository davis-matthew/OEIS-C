/*
 * A033182  Number of pairs (p,q) such that 5*p + 6*q = n.
 * https://oeis.org/A033182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/5) - floor((n-1)/6) (a(0) = 1)
 * Range:     u8 n <= 7649, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */
    idx_t t1 = n / 5;      /* floor(n/5) */
    idx_t t2 = (n - 1) / 6;/* floor((n-1)/6) */
    return (seq_t)(t1 - t2);
}
