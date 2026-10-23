/*
 * A028242  Follow n+1 by n. Also (essentially) Molien series of 2-dimensional
 *          quaternion group Q_8.
 * https://oeis.org/A028242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(2*n) = n+1, a(2*n+1) = n (equivalently a(n) = floor(n/2) +
 *            (n%2==0))
 * Range:     u8 n <= 308, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n/2) in seq_t. */
    seq_t half = (seq_t)n / 2;
    /* Add 1 only for even n. */
    if ((n & 1ULL) == 0)
        return half + 1;
    else
        return half;
}
