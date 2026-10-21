/*
 * A211867  a(n) = A097609(2*n-1,n), n>0; a(0)=1.
 * https://oeis.org/A211867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..⌊n/2⌋} (binomial(2*n, j) * binomial(n-j-1,
 *            n-2*j)) / 2, a(0) = 1. The binomials are computed iteratively to
 *            avoid factorials
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    idx_t half = n / 2;             /* floor(n/2) */
    seq_t sum   = 0;
    seq_t bin1  = (seq_t)(2ULL * n); /* binomial(2*n, 1) */
    seq_t bin2  = (seq_t)1;          /* binomial(n-2, n-2) for j = 1 */

    seq_t two_n = (seq_t)(2ULL * n); /* constant 2*n */

    for (idx_t j = 1; j <= half; ++j) {
        sum += bin1 * bin2;

        if (j == half) {
            break; /* no more terms */
        }

        /* Update bin1 to binomial(2*n, j+1) */
        bin1 = bin1 * (two_n - j) / (j + 1);

        /* Update bin2 to binomial(n-(j+1)-1, n-2*(j+1)) */
        seq_t b = (seq_t)(n - 2 * j);          /* b = n - 2*j          */
        seq_t a_val = (seq_t)(n - j - 1);        /* a = n - j - 1        */
        seq_t numer = b * (b - 1);              /* b*(b-1)              */
        seq_t denom = a_val * j;              /* a*j                  */
        bin2 = bin2 * numer / denom;
    }

    return sum / 2;
}
