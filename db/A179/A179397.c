/*
 * A179397  a(n) = prime(n)^2 mod prime(n-2).
 * https://oeis.org/A179397
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179397:  a(n) = prime(n)^2 mod prime(n-2)
 * (Offset 3 – a(3) is the first term)
 * The function keeps a static cache of primes and grows it as needed.
 * The product is computed using an unsigned 128‑bit intermediate when
 * available to avoid overflow of the intermediate square.
 */

#include <stdint.h>
#include <stdlib.h>

/* Utility prototypes supplied by the build system. */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 3; indices smaller than 3 are undefined. */
    if (n < 3)
        return 0;

    /* static cache of primes */
    static uint64_t *prime_cache = NULL; /* array of primes in order */
    static idx_t   prime_cache_size  = 0; /* number of primes cached */
    static idx_t   prime_cache_limit = 0; /* numeric limit used for cache */

    /* Ensure the cache contains at least the n‑th prime. */
    if (prime_cache_size < n)
    {
        /* Determine a new numeric bound that surely contains the nth prime. */
        idx_t new_limit = _prime_limit_for_idx(n);

        /* Rebuild the cache if the bound increased. */
        if (new_limit > prime_cache_limit)
        {
            free(prime_cache);
            build_primes_upto(new_limit, &prime_cache, &prime_cache_size);
            prime_cache_limit = new_limit;
        }
        else
        {
            /* Only need more primes; rebuild with the same limit. */
            free(prime_cache);
            build_primes_upto(prime_cache_limit, &prime_cache, &prime_cache_size);
        }
    }

    /* prime(n) is stored at index n-1 (because primes[0] = prime(1)). */
    uint64_t prime_n   = prime_cache[n - 1];
    uint64_t prime_nm2 = prime_cache[n - 3];

    /* Compute (prime_n)^2 mod prime_nm2 using an unsigned 128‑bit
     * intermediate when available, otherwise fall back to 64‑bit
     * modular multiplication. */
    seq_t term;
#if defined(__SIZEOF_INT128__)
    unsigned __int128 prod = (unsigned __int128)prime_n * prime_n;
    term = (seq_t)(prod % (unsigned __int128)prime_nm2);
#else
    /* 64‑bit fallback: use modular multiplication by repeated addition. */
    uint64_t m = prime_nm2;
    uint64_t t = 0;
    uint64_t a64 = prime_n % m;
    while (prime_n > 0)
    {
        if (prime_n & 1)
        {
            t = (t + a64) % m;
        }
        a64 = (a64 << 1) % m;
        prime_n >>= 1;
    }
    term = (seq_t)t;
#endif

    return term;
}
