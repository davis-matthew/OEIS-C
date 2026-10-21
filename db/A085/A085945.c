/*
 * A085945  Number of subsets of {1,2,...,n} with relatively prime elements.
 * https://oeis.org/A085945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * (2^{floor(n/k)} - 1) (Mobius
 *            inclusion–exclusion)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

static int *mu = NULL;          /* Mobius values for 1..current_limit */
static size_t mu_limit = 0;     /* Limit for which mu is computed */

/* Ensure that mu[1..n] are filled.  Recomputes up to n if necessary. */
static void ensure_mu(idx_t n)
{
    if (n <= mu_limit) return;

    size_t limit = (size_t)n;
    mu = (int *)realloc(mu, (limit + 1) * sizeof(int));
    mu[0] = 0;                /* unused */
    mu[1] = 1;

    /* Linear sieve for Mobius */
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, sizeof(unsigned char));
    unsigned int *primes = (unsigned int *)malloc((limit + 1) * sizeof(unsigned int));
    size_t prime_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = (unsigned int)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            size_t p = primes[j];
            size_t ip = i * p;
            if (ip > limit) break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }

    free(is_comp);
    free(primes);
    mu_limit = limit;
}

/* Main function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* defined arbitrarily for safety */

    ensure_mu(n);

    seq_t pos = 0;              /* Sum of terms with mu = +1 */
    seq_t neg = 0;              /* Sum of terms with mu = -1 */

    for (idx_t k = 1; k <= n; ++k) {
        int mk = mu[(size_t)k];
        if (mk == 0) continue;

        seq_t term = pow2(n / k);   /* 2^{floor(n/k)} */
        term -= 1;                  /* 2^{floor(n/k)} - 1 */

        if (mk == 1)
            pos += term;
        else /* mk == -1 */
            neg += term;
    }

    /* pos >= neg is guaranteed by the definition of the sequence. */
    return pos - neg;
}
