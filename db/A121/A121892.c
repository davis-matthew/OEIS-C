/*
 * A121892  Row sums of triangle in A066094.
 * https://oeis.org/A121892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=4; for n ≥ 3, a(n)=a(n‑1)*(2*n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)         return (seq_t)1;
    if (n == 1)         return (seq_t)2;
    seq_t res = (seq_t)4;        /* a(2) */

    if (n == 2)         return res;

    for (idx_t i = 3; i <= n; ++i)
        res *= (seq_t)(2 * i);

    return res;
}
