/*
 * A130088  a(n) = numerator of Product_{k=1..n} k^mu(n+1-k), where mu(k) =
 *          A008683(k).
 * https://oeis.org/A130088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of ∏_{k=1..n} k^{μ(n+1-k)} , μ = Möbius
 *            function (A008683)
 * Range:     u8 n <= 16, u16 n <= 24, u32 n <= 40, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* fast exponentiation for seq_t (integer type defined by the build) */
static seq_t pow_seq_t(seq_t base, int exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Return the n-th term of OEIS A130088.
   idx_t is an unsigned integer type (uint64_t).  seq_t may be an
   arbitrary‑precision integer class in C++ or an unsigned integer type
   in C. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for indices below the offset */

    /* cast to size_t for allocations – n is small enough for this use case */
    size_t N = (size_t)n;

    /* small prime factor array (spf) and Möbius array */
    idx_t *spf = (idx_t*)calloc(N + 1, sizeof(idx_t));
    if (!spf) return 0;
    int *mu   = (int*)calloc(N + 1, sizeof(int));
    if (!mu) { free(spf); return 0; }

    /* linear sieve to fill spf and mu */
    idx_t *primes = (idx_t*)malloc((N + 1) * sizeof(idx_t));
    if (!primes) { free(spf); free(mu); return 0; }
    size_t pcnt = 0;
    mu[1] = 1;

    for (size_t i = 2; i <= N; ++i)
    {
        if (spf[i] == 0)                /* i is prime */
        {
            spf[i] = (idx_t)i;
            primes[pcnt++] = (idx_t)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < pcnt &&
              primes[j] <= spf[i] &&
              i * primes[j] <= N; ++j)
        {
            size_t x = i * primes[j];
            spf[x] = primes[j];
            if (i % primes[j] == 0)
            {
                mu[x] = 0;
                break;
            }
            else
            {
                mu[x] = -mu[i];
            }
        }
    }

    /* exponent accumulator for each prime */
    int *exp = (int*)calloc(N + 1, sizeof(int));
    if (!exp) { free(spf); free(mu); free(primes); return 0; }

    /* process each term of the product */
    for (size_t m = 1; m <= N; ++m)
    {
        int e = mu[m];
        if (e == 0) continue;             /* skip square‑free multiples */

        size_t k = N + 1 - m;              /* the factor (n+1-m) */
        while (k > 1)
        {
            idx_t p = spf[k];
            int cnt = 0;
            while (k % p == 0)
            {
                k /= p;
                ++cnt;
            }
            exp[p] += e * cnt;             /* e = ±1 */
        }
    }

    /* assemble the reduced numerator from positive exponents */
    seq_t result = 1;
    for (size_t p = 2; p <= N; ++p)
    {
        int e = exp[p];
        if (e > 0)
            result *= pow_seq_t((seq_t)p, e);
    }

    /* clean up */
    free(spf);
    free(mu);
    free(primes);
    free(exp);

    return result;
}
