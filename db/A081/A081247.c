/*
 * A081247  Number of numbers 1<=k<=n with mu(k+n)=mu(k), where mu=A008683
 *          (Moebius function).
 * https://oeis.org/A081247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k : 1 ≤ k ≤ n , μ(k+n) = μ(k) }| (μ = mobius)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int8_t *mu_cache = NULL;          /* μ values up to current limit */
static size_t mu_limit = 0;              /* current maximum index covered */

static void compute_mu_up_to(idx_t limit)
{
    /* Recompute μ up to the requested limit using a linear sieve.
       The array is rebuilt entirely; this is cheap for the
       small limits (≤ 20000) used by the sequence. */
    size_t n = (size_t)limit;
    int8_t *mu_new = (int8_t *)malloc((n + 1) * sizeof(int8_t));
    if (!mu_new) exit(1);

    /* Temporary arrays for sieve */
    unsigned char *is_prime = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!is_prime) { free(mu_new); exit(1); }

    int *primes = (int *)malloc((n + 1) * sizeof(int));
    if (!primes) { free(mu_new); free(is_prime); exit(1); }

    size_t primes_cnt = 0;
    mu_new[0] = 0;
    mu_new[1] = 1;
    memset(is_prime + 2, 1, n - 1);

    for (size_t i = 2; i <= n; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = (int)i;
            mu_new[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = (size_t)primes[j];
            size_t ip = i * p;
            if (ip > n) break;
            is_prime[ip] = 0;
            if (i % p == 0) {
                mu_new[ip] = 0;
                break;
            } else {
                mu_new[ip] = -mu_new[i];
            }
        }
    }

    free(is_prime);
    free(primes);

    /* Replace old cache */
    if (mu_cache) free(mu_cache);
    mu_cache = mu_new;
    mu_limit = limit;
}

seq_t a(idx_t n)
{
    /* Ensure μ values are available up to 2n */
    unsigned long long max_needed = 2ULL * (unsigned long long)n;
    if (max_needed > mu_limit) compute_mu_up_to((idx_t)max_needed);

    /* Count matching μ values */
    seq_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (mu_cache[k] == mu_cache[k + n]) ++cnt;
    }
    return cnt;
}
