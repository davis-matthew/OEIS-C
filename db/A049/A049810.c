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
 * Helpers:   _mu_sieve
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   μ is the Möbius function.  The term 1 accounts for 0/1.  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/_mu_sieve.h"

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
