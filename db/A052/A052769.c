/*
 * A052769  E.g.f.: x^3*(exp(x)-1)^2.
 * https://oeis.org/A052769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - 16)*(n-2)*(n-1)*n / 8 (a(3) = 0)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 50, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Early zeros */
    if (n <= 2 || n == 3)
        return 0;

    /* compute 2^n as a seq_t value */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i)
        pow <<= 1;        /* unsigned left‑shift */

    /* (2^n - 16) / 8 , division by 8 is exact for n ≥ 3           */
    seq_t val = (pow - (seq_t)16) >> 3;   /* shift right by 3 */

    /* multiply the remaining factors using 128‑bit intermediate to
       avoid overflow (converts automatically to seq_t)           */
    seq_t prod = val * (n - 2);
    prod *= (n - 1);
    prod *= n;

    return prod;
}
