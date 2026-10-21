/*
 * A052754  Expansion of e.g.f.: (log(1-x))^2*x^2.
 * https://oeis.org/A052754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*(n-1)*(n-3)!*H_{n-3} (H_{m} = ∑_{k=1}^{m} 1/k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 4 the terms are zero. */

seq_t a(idx_t n)
{
    /* Small n give zero by definition. */
    if (n < 4)
        return (seq_t)0;

    /* m = n-3 is the index for the factorial and harmonic sum. */
    idx_t m = n - 3;

    /* Compute (n-3)! as a running product. */
    seq_t fac = (seq_t)1;
    for (idx_t i = 1; i <= m; ++i)
        fac *= (seq_t)i;          /* fac = i! after loop iteration i */

    /* Compute ∑_{k=1}^{m} fac / k.  Each division is exact because k | fac. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= m; ++k)
        sum += fac / (seq_t)k;

    /* 2 * n * (n-1) * sum.  Use seq_t conversions. */
    seq_t nseq = (seq_t)n;
    seq_t part = (seq_t)2 * nseq * (nseq - (seq_t)1);

    return part * sum;
}
