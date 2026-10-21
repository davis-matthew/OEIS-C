/*
 * A239968  a(n) = 0 unless n is a nonprime A018252(k) then a(n) = k.
 * https://oeis.org/A239968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is prime; otherwise a(n) = n - π(n), where π(n) is
 *            the number of primes ≤ n
 * Range:     u8 n <= 321, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for primality flags and prefix prime counts. */
static unsigned char *prime_flag = NULL;   /* prime_flag[i] = 1 if i is prime, 0 otherwise */
static idx_t *prime_cnt   = NULL;          /* prime_cnt[i] = number of primes ≤ i */
static idx_t cache_size = 0;               /* largest n for which cache is valid */

/* Ensure the sieve and prime counts are computed up to at least 'limit'. */
static void ensure_cache(idx_t limit)
{
    if (limit <= cache_size)
        return;

    size_t new_size = (size_t)(limit + 1);
    unsigned char *new_flag = (unsigned char *)realloc(prime_flag, new_size);
    idx_t *new_cnt = (idx_t *)realloc(prime_cnt, new_size * sizeof(idx_t));
    if (!new_flag || !new_cnt)
        abort(); /* insufficient memory */

    /* Initialize new entries as prime. */
    for (size_t i = cache_size; i < new_size; ++i)
        new_flag[i] = 1;

    /* 0 and 1 are not prime. */
    if (new_size > 0) new_flag[0] = 0;
    if (new_size > 1) new_flag[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (new_flag[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                new_flag[m] = 0;
        }
    }

    /* Compute prefix prime counts. */
    new_cnt[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        new_cnt[i] = new_cnt[i - 1] + (new_flag[i] ? 1 : 0);

    /* Commit updated structures. */
    prime_flag = new_flag;
    prime_cnt  = new_cnt;
    cache_size = limit;
}

/* Sequence function a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1 */

    ensure_cache(n);
    if (!prime_flag[n])
        return (seq_t)((seq_t)n - (seq_t)prime_cnt[n]); /* non‑prime */
    else
        return 0; /* prime */
}
