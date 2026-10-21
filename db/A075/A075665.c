/*
 * A075665  Sum of next n 4th powers.
 * https://oeis.org/A075665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n^9 + 90*n^7 + 123*n^5 + 20*n^3 - 8*n)/240
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 188,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*            = Σ_{i=n(n-1)/2+1}^{n(n-1)/2+n} i^4  (offset 1) */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset is 1 – n=0 never requested */
        return (seq_t)0;

    /* start = n*(n-1)/2 + 1, converted through seq_t to avoid overflow */
    seq_t start = ( (seq_t)n * ((seq_t)n - 1) ) / 2;
    start = start + (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        seq_t i   = start + (seq_t)k;   /* current index */
        seq_t i2  = i * i;              /* i^2  */
        seq_t i4  = i2 * i2;            /* i^4  */
        sum += i4;                      /* accumulate */
    }
    return sum;
}
