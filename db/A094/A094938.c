/*
 * A094938  a(n)=(-36^n/18)*B(2n,1/6)/B(2n,1/3) where B(n,x) is the n-th
 *          Bernoulli polynomial.
 * https://oeis.org/A094938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(n-1)/2 * (4^n - 2) (offset 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset starts at 1 */

    const seq_t C9 = (seq_t)9;
    const seq_t C4 = (seq_t)4;
    const seq_t C2 = (seq_t)2;

    /* compute 9^(n-1) */
    seq_t pow9 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow9 *= C9;

    /* compute 4^n */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow4 *= C4;

    /* (4^n - 2)/2  -- safe, because 4^n is even for n>=1 */
    seq_t t = (pow4 - C2) / C2;

    /* a(n) = 9^(n-1) * t  */
    return pow9 * t;
}
