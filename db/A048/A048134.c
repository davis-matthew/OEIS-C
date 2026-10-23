/*
 * A048134  Number of colors that can be mixed with up to n units of yellow,
 *          blue, red.
 * https://oeis.org/A048134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Σ_{d=1}^{n} μ(d) * (C(⌊n/d⌋ + 3, 3) – 1)
 * Range:     u8 n <= 10, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ(d) is the Möbius function. C(t, 3) = t*(t–1)*(t–2)/6.  Here the
   combination is evaluated as (m+3)*(m+2)*(m+1)/6 for m = ⌊n/d⌋.  */
#include <stdint.h>
#include <stdlib.h>

/* Lazy construction of μ up to the largest requested n */
static int *mu_global = NULL;  /* μ[0] unused, μ[1] = 1 */
static size_t mu_size = 0;     /* current allocated size of mu_global */

static void ensure_mu_limit(idx_t limit) {
    if (limit <= mu_size) return;

    /* Allocate new array (realloc is equivalent to malloc for simplicity) */
    int *new_mu = (int *)malloc((limit + 1) * sizeof(int));
    if (!new_mu) exit(1);
    new_mu[0] = 0;
    if (limit >= 1) new_mu[1] = 1;

    /* Sieve for μ (linear sieve) */
    char *is_composite = (char *)calloc(limit + 1, 1);
    if (!is_composite) exit(1);
    int *primes = (int *)malloc((limit + 1) * sizeof(int));
    if (!primes) exit(1);
    int primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[primes_cnt++] = (int)i;
            new_mu[i] = -1;
        }
        for (int pj = 0; pj < primes_cnt; ++pj) {
            int p = primes[pj];
            idx_t ip = i * p;
            if (ip > limit) break;
            is_composite[ip] = 1;
            if (i % p == 0) {
                new_mu[ip] = 0;
                break;
            } else {
                new_mu[ip] = -new_mu[i];
            }
        }
    }

    free(is_composite);
    free(primes);

    /* Replace global data */
    free(mu_global);
    mu_global = new_mu;
    mu_size = limit;
}

/* Compute f(m) = C(m+3, 3) – 1 = ((m+3)*(m+2)*(m+1))/6 – 1 */
static inline seq_t f_value(idx_t m) {
    seq_t t = (seq_t)m;
    seq_t res = ((t + 3) * (t + 2) * (t + 1)) / 6;
    return res - 1;
}

seq_t a(idx_t n) {
    /* Make sure μ is available up to n */
    ensure_mu_limit(n);

    seq_t total = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int mu_d = mu_global[d];
        if (mu_d == 0) continue;
        idx_t m = n / d;
        seq_t fv = f_value(m);
        total += (seq_t)mu_d * fv;
    }
    return total;
}
