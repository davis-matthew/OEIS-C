/*
 * A174942  a(n) = Sum_{k<=n} A007955(k) * A008683(n-k+1) = Sum_{k<=n}
 *          A007955(k) * mu(n-k+1), where A007955(m) = product of divisors of
 *          m.
 * https://oeis.org/A174942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=1..n} A007955(k) * μ(n-k+1) * = Σ_{k=1..n}
 *            ProdDiv(k) * mobius(n-k+1)
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* integer power for seq_t, exponent fits in int */
static seq_t ipow(seq_t base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i) res *= base;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n starts from 1 in OEIS */

    size_t N = (size_t)n;
    /* allocate required arrays */
    int8_t *mu   = (int8_t *)calloc(N + 1, sizeof(int8_t));
    seq_t *prod  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    int *is_comp = (int *)calloc(N + 1, sizeof(int));
    int *primes  = (int *)malloc((N + 1) * sizeof(int));
    int primes_cnt = 0;

    mu[1] = 1; /* mobius(1) = 1 */

    /* linear sieve for primes and mobius */
    for (size_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (int pi = 0; pi < primes_cnt; ++pi) {
            size_t p = (size_t)primes[pi];
            size_t j = i * p;
            if (j > N) break;
            is_comp[j] = 1;
            if (i % p == 0) {
                mu[j] = 0;
                break;
            } else {
                mu[j] = -mu[i];
            }
        }
    }

    /* compute product of divisors for each k */
    for (size_t k = 1; k <= N; ++k) {
        if (k == 1) {      /* prod[1] = 1 */
            prod[k] = 1;
            continue;
        }
        int temp = (int)k;
        int d = 1;                 /* number of divisors */
        int ps[10];
        int exps[10];
        int pc = 0;

        while (temp > 1) {
            int p = 0;
            for (int pi = 0; pi < primes_cnt &&
                (long long)primes[pi] * primes[pi] <= temp; ++pi) {
                if (temp % primes[pi] == 0) {
                    p = primes[pi];
                    break;
                }
            }
            if (p == 0) p = temp;  /* temp is prime */
            int a = 0;
            while (temp % p == 0) {
                temp /= p;
                ++a;
            }
            d *= (a + 1);
            ps[pc] = p;
            exps[pc] = a;
            ++pc;
        }

        seq_t prod_val = 1;
        for (int i = 0; i < pc; ++i) {
            int a = exps[i];
            int e = (a * d) / 2;  /* integer exponent */
            seq_t pow_val = ipow((seq_t)ps[i], e);
            prod_val *= pow_val;
        }
        prod[k] = prod_val;
    }

    /* convolution with mobius */
    seq_t res = 0;
    for (size_t k = 1; k <= N; ++k) {
        size_t idx = N - k + 1;          /* (n-k+1) */
        int8_t mu_val = mu[idx];
        if (mu_val != 0) {
            res += prod[k] * (seq_t)mu_val;
        }
    }

    /* free allocated memory */
    free(mu);
    free(prod);
    free(is_comp);
    free(primes);

    return res;
}
