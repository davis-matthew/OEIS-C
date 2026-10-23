/*
 * A195019  Multiples of 3 and of 4 interleaved: a(2*n-1) = 3*n, a(2*n) = 4*n.
 * https://oeis.org/A195019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n-1) = 3*n, a(2*n) = 4*n
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n >= 1 */
    /* For odd n (n = 2*k-1): a = 3*k,  k = (n+1)/2 */
    if (n & 1)
    {
        seq_t k = (seq_t)((n + 1) / 2);
        return 3 * k;
    }
    /* For even n (n = 2*k): a = 4*k, k = n/2 */
    else
    {
        seq_t k = (seq_t)(n / 2);
        return 4 * k;
    }
}
