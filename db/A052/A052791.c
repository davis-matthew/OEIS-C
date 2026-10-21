/*
 * A052791  a(n) = 3^(n-3)*n*(n-1)*(n-2).
 * https://oeis.org/A052791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3^(n-3) * n * (n-1) * (n-2) (a(0)=a(1)=a(2)=0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* compute n*(n-1)*(n-2) using seq_t arithmetic */
    seq_t prod = (seq_t)n * ((seq_t)(n - 1)) * ((seq_t)(n - 2));

    /* compute 3^(n-3) by repeated multiplication (fast enough for n≤10000) */
    seq_t pow = 1;
    for (idx_t i = 0; i < n - 3; ++i)
        pow *= 3;

    return prod * pow;
}
