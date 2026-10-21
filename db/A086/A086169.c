/*
 * A086169  Sum of the first n twin prime pairs.
 * https://oeis.org/A086169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of first n twin prime pairs. a(n) = Σ_{k=1..n} (p_k +
 *            (p_k + 2)), where p_k is the first member of the k‑th twin‑prime
 *            pair
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of the sequence */
    static seq_t *cache  = NULL;    /* 1‑based: cache[1] = a(1) */
    static size_t cache_size = 0;   /* number of precomputed terms */

    /* build the cache on first call */
    if (!cache)
    {
        /* upper limit that easily contains many twin primes */
        const idx_t LIMIT = 8000000;    /* ~ 41 000 twin primes, plenty for n ≤ 10 000 */

        /* simple sieve of Eratosthenes */
        char *prime = (char*)malloc((LIMIT + 3)*sizeof(char));
        if (!prime) return 0;              /* allocation failure – unlikely */
        for (idx_t i = 0; i <= LIMIT + 2; ++i) prime[i] = 1;
        prime[0] = prime[1] = 0;
        for (idx_t i = 2; i * i <= LIMIT; ++i)
        {
            if (prime[i])
            {
                for (idx_t j = i * i; j <= LIMIT; j += i)
                    prime[j] = 0;
            }
        }

        /* count twin primes to size the cache */
        size_t max_pairs = 0;
        for (idx_t i = 2; i + 2 <= LIMIT; ++i)
            if (prime[i] && prime[i + 2])
                ++max_pairs;

        /* allocate cache (index 0 unused) */
        cache = (seq_t*)malloc((max_pairs + 1) * sizeof(seq_t));
        if (!cache)
        {
            free(prime);
            return 0;
        }
        cache[0] = 0;  /* placeholder, unused */

        /* compute cumulative sums */
        seq_t sum = 0;
        size_t idx = 0;
        for (idx_t i = 2; i + 2 <= LIMIT; ++i)
            if (prime[i] && prime[i + 2])
            {
                sum += (seq_t)i + (seq_t)(i + 2);
                ++idx;
                cache[idx] = sum;      /* cache[idx] = a(idx) */
            }

        cache_size = idx;        /* actual number of computed terms */
        free(prime);
    }

    /* return the requested term */
    if (n >= 1 && n <= cache_size)
        return cache[n];

    /* out of range: return 0 (should not happen for valid tests) */
    return 0;
}
