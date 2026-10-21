/*
 * A069071  a(n) = (2*n + 1)*((2*n + 1)^4 + 4).
 * https://oeis.org/A069071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n + 1)*((2*n + 1)^4 + 4)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 41, u64 n <= 3565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t   = (seq_t)n * 2 + 1;   /* 2*n + 1 */
    seq_t t2  = t * t;              /* (2*n + 1)^2 */
    seq_t t4  = t2 * t2;            /* (2*n + 1)^4 */
    seq_t res = t * (t4 + 4);       /* final result */
    return res;
}
