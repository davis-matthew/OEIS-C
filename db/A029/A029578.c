/*
 * A029578  The natural numbers interleaved with the even numbers.
 * https://oeis.org/A029578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k) = k, a(2*k-1) = 2*k-2
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even indices: a(n) = n/2 */
    if ((n & (idx_t)1) == 0)
        return (seq_t)(n >> 1);

    /* Odd indices: a(n) = n - 1 */
    return (seq_t)(n - 1);
}
