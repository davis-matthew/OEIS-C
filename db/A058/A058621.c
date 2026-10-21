/*
 * A058621  a(n) = 1/2*binomial(2*n,n) -
 *          (1+(-1)^n)/4*(binomial(n,floor(n/2)))^2.
 * https://oeis.org/A058621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor((n-1)/2)} binomial(n,i)^2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t lim = (n - 1) / 2;            /* upper bound for i   */
    seq_t sum = 0;
    seq_t bin = 1;                      /* binomial(n,0) */

    for (idx_t i = 0; i <= lim; ++i) {
        sum += bin * bin;               /* add binomial(n,i)^2 */

        if (i < lim) {                  /* update to binomial(n,i+1) */
            bin = bin * (seq_t)(n - i) / (seq_t)(i + 1);
        }
    }

    return sum;
}
