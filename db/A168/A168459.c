/*
 * A168459  a(n) = (10*n + 5*(-1)^n - 3)/2.
 * https://oeis.org/A168459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1 + 10*floor(n/2)
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 the value is
       undefined; we simply return 0 to avoid unsigned underflow. */
    if (n == 0) {
        return 0;
    }
    /* floor(n/2) is just integer division for unsigned types. */
    idx_t half = n / 2;
    /* Compute a(n) = 1 + 10 * (n/2) in the integral type seq_t. */
    seq_t result = (seq_t)1 + (seq_t)10 * (seq_t)half;
    return result;
}
