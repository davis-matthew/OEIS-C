/*
 * A009281  Expansion of e.g.f. exp(x)*cosh(log(1+x)).
 * https://oeis.org/A009281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0}^{n} (-1)^k · (n! / (n-k)!) + n + 1 all divided by
 *            2 Equivalently a(n) = [1 + n + Σ (-1)^k · P(n,k)] / 2 where
 *            P(n,k) = n·(n-1)·...·(n-k+1) (with P(n,0)=1)
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)1;          /* the constant 1 */
    if (n >= 1) sum += (seq_t)n;   /* the "+ n" term for n≥1 */

    seq_t cur = (seq_t)1;          /* P(n,0) */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* add or subtract the current product depending on k parity */
        if ((k & 1) == 0)
            sum += cur;
        else
            sum -= cur;

        /* update cur = P(n,k+1) */
        if (k < n)
            cur *= (seq_t)(n - k);
    }

    /* the sum is guaranteed to be even; divide by two to obtain a(n) */
    return sum / (seq_t)2;
}
