/*
 * A031389  a(n) = prime(8*n-3).
 * https://oeis.org/A031389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(8*n-3)
 * Range:     u8 n <= 7, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* offset is 1 */

    idx_t k = (idx_t)8 * n - (idx_t)3;        /* index of the required prime */

    /* Rough upper bound for the k‑th prime.  For k up to 10000 this is far above
       the actual value, so the sieve is always safe. */
    idx_t limit = k * (idx_t)20 + (idx_t)10;

    /* Cached primes: only rebuilt if limit grows. */
    static idx_t *prime_cache  = NULL;
    static size_t prime_count  = 0;
    static idx_t  prime_limit  = 0;

    if (limit > prime_limit) {
        /* free previous cache */
        if (prime_cache) free(prime_cache);

        size_t sieve_size   = (size_t)(limit + 1);
        bool *sieve         = (bool *)malloc(sieve_size);
        if (!sieve) return (seq_t)0;

        /* initialise all entries to true */
        for (size_t i = 0; i < sieve_size; ++i) sieve[i] = true;
        if (sieve_size > 0) sieve[0] = false;
        if (sieve_size > 1) sieve[1] = false;

        /* Eratosthenes sieve */
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (sieve[(size_t)i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[(size_t)j] = false;
            }
        }

        /* collect primes */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (sieve[(size_t)i]) ++cnt;

        prime_cache = (idx_t *)malloc(cnt * sizeof(idx_t));
        if (!prime_cache) { free(sieve); return (seq_t)0; }

        size_t pos = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (sieve[(size_t)i]) prime_cache[pos++] = i;

        prime_count = cnt;
        prime_limit = limit;

        free(sieve);
    }

    /* k is 1‑based; array is 0‑based */
    if ((idx_t)k - (idx_t)1 >= prime_count)
        return (seq_t)0;           /* safety check */

    return (seq_t)prime_cache[k - 1];
}
