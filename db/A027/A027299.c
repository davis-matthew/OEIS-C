/*
 * A027299  a(n) = Sum_{k=0..m} (k+1) * A026022(n, m-k), where m=n for n=0,1
 *          and m = floor((n+3)/2) for n >= 2.
 * https://oeis.org/A027299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+1) + binom(n+1, (n-1)/2) for odd n>=3; a(0)=1,
 *            a(1)=3
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute 2^exp as a seq_t */
static seq_t pow2_u64(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i) r *= 2;
    return r;
}

/* compute binomial coefficient C(n,k) multiplicatively */
static seq_t binomial_multiplicative(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = r * (seq_t)(n - k + i);    /* multiply by numerator */
        r = r / (seq_t)i;              /* divide by denominator */
    }
    return r;
}

/* OEIS A027299: a(n) = Sum_{k=0..m} (k+1)*A026022(n,m-k) */
seq_t a(idx_t n)
{
    /* special small cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;

    /* base value 2^(n+1) */
    seq_t result = pow2_u64(n + 1);

    /* add the binomial term only for odd n >= 3 */
    if ((n & 1u) != 0u) {
        idx_t k = (n - 1) >> 1;                     /* (n-1)/2 */
        seq_t add = binomial_multiplicative(n + 1, k);
        result += add;
    }

    return result;
}
