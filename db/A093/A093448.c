/*
 * A093448  Rows sums of the triangle A093447.
 * https://oeis.org/A093448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = sum_{k=1}^{n} \prod_{i=0}^{k-1} ( ((n*(n+1))/2) -
 *            (k*(k+1))/2 + 1 + i ) * i.e. partition the consecutive integers
 *            1,2,…,n(n+1)/2 into groups of * lengths n,n-1,…,1 and sum the
 *            products of the numbers in each group
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t cur = 1;              /* next integer to use */

    for (idx_t len = n; len > 0; --len) {
        seq_t prod = 1;
        idx_t end = cur + len;  /* end is exclusive */

        for (idx_t i = cur; i < end; ++i) {
            prod *= (seq_t)i;
        }
        sum += prod;
        cur = end;
    }
    return sum;
}
