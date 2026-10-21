/*
 * A217467  a(1) = 1; for n > 1, the maximum exponent k such that n^k divides
 *          the double factorial n!!.
 * https://oeis.org/A217467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum k such that n^k divides n!!
 * Range:     u8 n <= 2303, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* exponent of p in n! (Legendre's formula) */
static idx_t exp_factorial(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n) {
        n /= p;
        res += n;
    }
    return res;
}

/* exponent of p in n!! */
static idx_t exp_double_factorial(idx_t n, idx_t p)
{
    if (n % 2 == 0) {                   /* even n: n!! = 2^(n/2) * (n/2)! */
        idx_t half = n >> 1;
        if (p == 2) {
            return half + exp_factorial(half, 2);
        } else {
            return exp_factorial(half, p);
        }
    } else {                            /* odd n: n!! = n! / (2^((n-1)/2) * ((n-1)/2)!) */
        if (p == 2) return 0;           /* 2 does not divide an odd double factorial */
        idx_t half = (n - 1) >> 1;      /* (n-1)/2 */
        return exp_factorial(n, p) - exp_factorial(half, p);
    }
}

seq_t a(idx_t n)
{
    /* offset 1: a(1)=1 by definition */
    if (n <= 1) return (seq_t)1;

    idx_t min_k = (idx_t)-1;            /* start with maximum value */
    idx_t m = n;

    /* trial division factorisation of n */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            idx_t exp_dbl = exp_double_factorial(n, p);
            idx_t k = exp_dbl / e;
            if (k < min_k) min_k = k;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        idx_t exp_dbl = exp_double_factorial(n, m);
        idx_t k = exp_dbl;               /* e = 1 for a prime factor */
        if (k < min_k) min_k = k;
    }

    return (seq_t)min_k;
}
