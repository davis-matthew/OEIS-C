/*
 * A126804  a(n) = (2n)! / (n-1)!.
 * https://oeis.org/A126804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{i=n..2n} i
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    seq_t prod = 1;
    idx_t end = n + n;             /* 2*n, safe for n up to 10000 */

    for (idx_t i = n; i <= end; ++i)
        prod = prod * (seq_t)i;

    return prod;
}
