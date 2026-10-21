/*
 * A083363  Diagonal of table A083362.
 * https://oeis.org/A083363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0 set k = floor((n+1)/2). Then a(2k-1) = 8*k*k - k
 *            a(2k) = 8*k*k + 3*k
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;

    /* Compute k = floor((n+1)/2) */
    seq_t k = n / 2u;
    if (n & 1u)  /* odd n -> (n+1)/2 = n/2 + 1 */
        ++k;

    /* Base term 8*k*k */
    seq_t base = (seq_t)8 * k * k;

    if (n & 1u) /* odd index */
        return base - k;      /* 8*k*k - k */
    else        /* even index */
        return base + (seq_t)3 * k;   /* 8*k*k + 3*k */
}
