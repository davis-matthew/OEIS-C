/*
 * A225168  Denominators of the sequence s(n) of the sum resp. product of
 *          fractions f(n) defined recursively by f(1) = 9/1; f(n+1) is chosen
 *          so that the sum and the product of the first n terms of the
 *          sequence are equal.
 * https://oeis.org/A225168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^(2^(n-1))*b(n) where b(1)=1/9 and b(n+1)=b(n)-b(n)^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     a(n) satisfies a(1)=1 and for n≥1  a(n+1)=a(n)*(9^(2^(n-1))-a(n))  */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; this case never used */
    seq_t val = 1;                 /* a(1) */
    seq_t pow = (seq_t)9;          /* pow = 9^(2^(current-1)) */

    for (idx_t i = 1; i < n; ++i) {
        seq_t diff = pow - val;    /* pow > val for all used n */
        seq_t prod = val * diff;   /* compute next a */
        val = prod;
        pow = pow * pow;          /* update pow to next power of 9 */
    }
    return val;
}
