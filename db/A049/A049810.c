/*
 * A049810  a(n)=number of Farey fractions of order n that are <=1/6; cf.
 *          A049805.
 * https://oeis.org/A049810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{q=1..n} Σ_{d|q} μ(d) * ⌊⌊q/6⌋ / d⌋
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   μ is the Möbius function.  The term 1 accounts for 0/1.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* fast generation of μ[1..N] (linear sieve) */
static void _mu_sieve(idx_t N, int8_t **mu_out, size_t *size_out)
{
    /* allocate new buffer */
    int8_t *mu = (int8_t *)malloc((N + 1) * sizeof(int8_t));
    uint8_t *is_comp = (uint8_t *)calloc((N + 1), sizeof(uint8_t));
    int *primes = (int *)malloc((N + 1) * sizeof(int));
    size_t pc = 0;
    mu[0] = 0;
    mu[1] = 1;
    for (idx_t i = 2; i <= N; ++i) {
        if (!is_comp[i]) {
            primes[pc++] = (int)i;
            mu[i] = -1;
        }
        for (size_t k = 0; k < pc; ++k) {
            int p = primes[k];
            idx_t v = i * (idx_t)p;
            if (v > N) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;
                break;
            } else {
                mu[v] = -mu[i];
            }
        }
    }
    free(is_comp);
    free(primes);
    *mu_out = mu;
    *size_out = N + 1;
}

/* static cache for μ to avoid recomputation */
static int8_t *mu_cache = NULL;
static size_t mu_cache_size = 0;

/* ensure μ array computed up to n */
static void _ensure_mu(idx_t n)
{
    if (mu_cache_size > 0 && mu_cache_size > n)
        return;
    int8_t *new_mu = NULL;
    size_t new_size = 0;
    _mu_sieve(n, &new_mu, &new_size);
    free(mu_cache);          /* free old if any */
    mu_cache = new_mu;
    mu_cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* behaviour below offset is irrelevant           */

    /* compute μ up to n once (or grow if needed) */
    _ensure_mu(n);

    int64_t tot = 0;          /* signed 64‑bit accumulator   */
    for (idx_t q = 1; q <= n; ++q) {
        idx_t m = q / 6;      /* floor(q/6)                  */
        if (m == 0) continue; /* no p>0 possible              */

        /* enumerate divisors of q */
        for (idx_t d = 1; d * d <= q; ++d) {
            if (q % d == 0) {
                int8_t mu1 = mu_cache[d];
                if (mu1 != 0) {
                    tot += (int64_t)mu1 * (m / d);
                }
                idx_t d2 = q / d;
                if (d2 != d) {
                    int8_t mu2 = mu_cache[d2];
                    if (mu2 != 0) {
                        tot += (int64_t)mu2 * (m / d2);
                    }
                }
            }
        }
    }
    /* add the fraction 0/1 */
    return (seq_t)(tot + 1);
}
