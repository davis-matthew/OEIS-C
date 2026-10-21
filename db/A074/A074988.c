/*
 * A074988  Numbers n such that the k-th binary digit of n equals mu(k)^2 for
 *          k=1 up to A029837(n+1).
 * https://oeis.org/A074988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2*a(n-1) + mu(n)^2 (a(1) = 1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  mu(k) is the Möbius function; mu(k)^2 is 1 iff k is square‑free, otherwise 0. */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for already computed values */
static idx_t  cached_n  = 0;   /* largest n for which mu[1..cached_n] and a[1..cached_n] are filled */
static int8_t *mu_cache  = NULL;   /* mu array, size cached_n+1, index 1..cached_n */
static seq_t *a_cache   = NULL;    /* a array, size cached_n+1, index 1..cached_n */

/* Ensure that mu and a are available for all indices up to n. */
static void
ensure_cached(idx_t n)
{
    if (n <= cached_n)
        return;

    /* Allocate new storage big enough */
    int8_t  *new_mu = (int8_t *)malloc((n + 1) * sizeof(int8_t));
    seq_t   *new_a  = (seq_t  *)malloc((n + 1) * sizeof(seq_t));

    /* Linear sieve for μ up to n */
    unsigned char *is_comp = (unsigned char *)calloc(n + 1, sizeof(unsigned char));
    idx_t *primes = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    size_t prime_cnt = 0;

    /* μ(1) = 1 */
    new_mu[1] = 1;

    for (idx_t i = 2; i <= n; ++i)
    {
        if (!is_comp[i])
        {
            primes[prime_cnt++] = i;
            new_mu[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j)
        {
            idx_t p = primes[j];
            idx_t ip = i * p;
            if (ip > n)
                break;
            is_comp[ip] = 1;
            if (i % p == 0)
            {
                new_mu[ip] = 0;
                break;
            }
            else
            {
                new_mu[ip] = -new_mu[i];
            }
        }
    }

    free(is_comp);
    free(primes);

    /* Compute a[1..n] using the recurrence */
    new_a[0] = 0;          /* unused, keep 0‑based array robust */
    new_a[1] = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t mu_i = (seq_t)new_mu[i];   /* μ(i) is -1, 0, or 1 */
        new_a[i] = (seq_t)2 * new_a[i - 1] + mu_i * mu_i;
    }

    /* Free old cache */
    free(mu_cache);
    free(a_cache);

    /* Update cache pointers and size */
    mu_cache  = new_mu;
    a_cache   = new_a;
    cached_n  = n;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for non‑positive indices  */

    ensure_cached(n);
    return a_cache[n];
}
