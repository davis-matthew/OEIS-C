/*
 * A228600  The Szeged index of the n-sunlet graph (n>=3).
 * https://oeis.org/A228600
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*floor(n/2)^2 + 2*n^2 - n (equivalently n^3 if n odd,
 *            n^3+2n^2-n if n even)
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;          /* cast to seq_t */
    seq_t half = tn >> 1;           /* floor(n/2) */
    seq_t sqHalf = half * half;     /* (floor(n/2))^2 */
    seq_t res = 4 * tn * sqHalf + 2 * tn * tn - tn;
    return res;
}
