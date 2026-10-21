/*
 * A206591  E.g.f.: Sum_{n>=0} x^(n^2)*exp(n^2*x).
 * https://oeis.org/A206591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..⌊√n⌋} k^{2(n-k^2)} / (n-k^2)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 1) <= n) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* compute floor(sqrt(n)) */
    idx_t kmax = floor_isqrt_u64(n);

    seq_t sum = 0;

    for (idx_t k = 1; k <= kmax; ++k)
    {
        idx_t k2  = k * k;              /* k² */
        idx_t m   = n - k2;             /* exponent for power, and factorial denominator */

        /* n! / m! = product_{i=m+1}^{n} i */
        idx_t start = n - k2 + 1;       /* = m + 1 */
        seq_t prod = 1;
        for (idx_t j = 0; j < k2; ++j)
            prod *= (seq_t)(start + j);

        /* k^{2m}  (exponent = 2*m) */
        idx_t exp = m << 1;              /* 2*m, safe for our n range */
        seq_t pow_term = 1;
        seq_t base = (seq_t)k;         /* base = k */
        idx_t e = exp;
        while (e)
        {
            if (e & 1U) pow_term *= base;
            base *= base;
            e >>= 1;
        }

        prod *= pow_term;
        sum += prod;
    }
    return sum;
}
