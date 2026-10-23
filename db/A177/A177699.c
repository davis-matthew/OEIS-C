/*
 * A177699  Expansion of e.g.f. log(1+x) * sinh(x).
 * https://oeis.org/A177699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^n * Sum_{k=1..floor(n/2)} (n-2k)! * binomial(n,
 *            2k-1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t maxk = n / 2;
    seq_t sum = 0;

    /* Compute (n-2)! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n - 2; ++i)
        fact *= (seq_t)i;

    seq_t binom = (seq_t)n;      /* binomial(n,1) */
    idx_t m = 1;                 /* current odd index */

    for (idx_t k = 1; k <= maxk; ++k) {
        sum += fact * binom;

        if (k < maxk) {
            /* Update factorial: (n-2k-2)! = (n-2k)! / [(n-2k)*(n-2k-1)] */
            seq_t denom = (seq_t)(n - 2 * k) * (seq_t)(n - 2 * k - 1);
            fact /= denom;

            /* Update binomial: C(n, m+2) = C(n, m) * (n-m)/(m+1) * (n-m-1)/(m+2) */
            seq_t temp = binom * (seq_t)(n - m) / (seq_t)(m + 1);
            binom = temp * (seq_t)(n - m - 1) / (seq_t)(m + 2);
            m += 2;
        }
    }

    if (n & 1)
        sum = -sum;

    return sum;
}
