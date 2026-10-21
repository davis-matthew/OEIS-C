/*
 * A210379  Number of 2 X 2 matrices with all terms in {0,1,...,n} and odd
 *          trace.
 * https://oeis.org/A210379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2 * floor((n+1)^2 / 2)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 303, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the value type first */
    seq_t m = (seq_t)n + 1;      /* m = n + 1 */
    seq_t m2 = m * m;            /* m2 = (n+1)^2 */
    seq_t half = m2 / 2;         /* floor((n+1)^2 / 2) */
    return m2 * half;            /* (n+1)^2 * floor((n+1)^2 / 2) */
}
