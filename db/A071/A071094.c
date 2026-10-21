/*
 * A071094  Number of ways to tile hexagon of edges n, n, n+1, n, n, n+1 with
 *          diamonds of side 1.
 * https://oeis.org/A071094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=0}^{n-1} Product_{j=0}^{n-1} Product_{k=0}^{n}
 *            (i + j + k + 2) / (i + j + k + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t pow_big(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t limit = 3 * n + 2;                 /* maximum number to factorize */
    /* Smallest prime factor sieve – works up to limit <= 3*n+2 (≈30 000 for n=10 000) */
    int *spf = (int *)malloc((size_t)(limit + 1) * sizeof(int));
    for (idx_t i = 0; i <= limit; ++i) spf[i] = (int)i;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (int)i) {              /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == (int)j) spf[j] = (int)i;
        }
    }

    /* Exponent table for primes up to limit.  init to 0 */
    int *expcnt = (int *)calloc((size_t)(limit + 1), sizeof(int));

    /* Count exponents from the product formula */
    for (idx_t s = 0; s <= 2 * n - 2; ++s) {
        idx_t mult = (s < n) ? (s + 1) : (2 * n - 1 - s);   /* multiplicity C(s) */
        idx_t num = s + n + 2;
        idx_t den = s + 1;

        /* factorise numerator and add exponent */
        idx_t x = num;
        while (x > 1) {
            int p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            expcnt[p] += cnt * mult;
        }

        /* factorise denominator and subtract exponent */
        x = den;
        while (x > 1) {
            int p = spf[x];
            int cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            expcnt[p] -= cnt * mult;
        }
    }

    /* Multiply all prime powers to get the final result */
    seq_t result = 1;
    for (idx_t p = 2; p <= limit; ++p) {
        int e = expcnt[p];
        if (e > 0) {
            seq_t term = pow_big((seq_t)p, (idx_t)e);
            result = result * term;
        }
    }

    free(spf);
    free(expcnt);
    return result;
}
