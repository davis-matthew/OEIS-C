/*
 * A165424  a(1) = 1, a(2) = 6, a(n) = product of the previous terms for n >=
 *          3.
 * https://oeis.org/A165424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the previous terms, with a(1)=1, a(2)=6; for
 *            n>=3 this simplifies to a(n) = a(n-1)^2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* undefined offset, return 1 for safety */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    seq_t res = (seq_t)6;          /* a(3) */
    for (idx_t i = 3; i < n; ++i)
        res = res * res;           /* a(i) = a(i-1)^2 for i>=4 */
    return res;
}
