/*
 * A206526  a(n) = 137*(n-1) - a(n-1) with n>1, a(1)=31.
 * https://oeis.org/A206526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n/2)*137 + (n%2 ? 31 : -31)
 * Range:     u8 n <= 4, u16 n <= 957, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 the value is undefined,
     * but we return 0 to avoid arithmetic underflow. */
    if (n == 0)
        return (seq_t)0;

    idx_t half = n >> 1;             /* floor(n/2) */
    seq_t base = half * 137;         /* base term */

    if ((n & 1) == 0)                /* even n */
        return base - (seq_t)31;     /* a(2k) = k*137 – 31 */
    else
        return base + (seq_t)31;     /* a(2k+1) = k*137 + 31 */
}
