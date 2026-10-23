/*
 * A129326  a(n) = (2*n+1)*(n-1)!.
 * https://oeis.org/A129326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+1)*(n-1)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0, but harmless */

    seq_t fact = 1;
    for (idx_t i = 1; i < n; ++i)
        fact *= (seq_t)i;

    seq_t coeff = (seq_t)((seq_t)n * 2 + 1);
    return fact * coeff;
}
