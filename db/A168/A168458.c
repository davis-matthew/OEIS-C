/*
 * A168458  a(n) = 7 + 10*floor((n-1)/2).
 * https://oeis.org/A168458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 + 10*floor((n-1)/2)
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n == 0 the behaviour is undefined for the sequence,
       but we return 0 to avoid unsigned underflow. */
    if (n == 0u)
        return (seq_t)0;
    seq_t k = ((seq_t)(n - 1u)) / 2u;   /* floor((n-1)/2) */
    return (seq_t)7 + (seq_t)10 * k;
}
