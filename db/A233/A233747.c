/*
 * A233747  Numbers n such that n+k, n+k^2 and n+k^4, are all prime, where
 *          k=4.
 * https://oeis.org/A233747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 700, u32 n <= 5250, u64 n <= 5250,
 *            u128 n <= 5250, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233747: numbers n such that n+4, n+16 and n+256 are all prime.
   Computed by an incremental sieve and lazy caching of terms. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ----- integer square root (floor) for 64‑bit values ----- */
static idx_t int_floor_sqrt(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;            /* high > sqrt(UINT64_MAX) */
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* ----- dynamic prime sieve ----- */
static uint8_t *prime_sieve = NULL;   /* 1 = prime, 0 = composite */
static idx_t prime_limit = 0;         /* inclusive maximum index */

static void ensure_prime_sieve(idx_t min_limit)
{
    if (min_limit <= prime_limit) return;

    idx_t new_limit = prime_limit ? prime_limit * 2 : 1000;
    if (new_limit < min_limit) new_limit = min_limit;
    new_limit += 1000;  /* cushion */

    uint8_t *new_sieve = (uint8_t*)calloc((size_t)new_limit + 1, 1);
    if (!new_sieve) exit(1);

    memset(new_sieve + 2, 1, (size_t)new_limit - 1);   /* mark >=2 as prime */

    idx_t sqrt_limit = int_floor_sqrt(new_limit);
    for (idx_t p = 2; p <= sqrt_limit; ++p)
    {
        if (new_sieve[p])
        {
            for (idx_t j = p * p; j <= new_limit; j += p)
                new_sieve[j] = 0;
        }
    }

    free(prime_sieve);
    prime_sieve = new_sieve;
    prime_limit  = new_limit;
}

/* ----- caching of found terms ----- */
#ifdef __cplusplus
static std::vector<seq_t> cache;
#else
static seq_t *cache_arr = NULL;
static size_t cache_len = 0, cache_cap = 0;
static void cache_push(seq_t val)
{
    if (cache_len == cache_cap)
    {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache_arr = (seq_t*)realloc(cache_arr, cache_cap * sizeof(seq_t));
    }
    cache_arr[cache_len++] = val;
}
#endif

/* ----- static state used by the function ----- */
static idx_t next_candidate = 1;

/* ----- main function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, return 0 for safety */

    /* Expand cache until we have at least n terms */
    while (1)
    {
#ifdef __cplusplus
        if (cache.size() >= n) break;
#else
        if (cache_len >= n) break;
#endif

        /* Search for the next qualifying n */
        while (1)
        {
            ensure_prime_sieve(next_candidate + 256);

            if (prime_sieve[next_candidate + 4] &&
                prime_sieve[next_candidate + 16] &&
                prime_sieve[next_candidate + 256])
            {
#ifdef __cplusplus
                cache.push_back((seq_t)next_candidate);
#else
                cache_push((seq_t)next_candidate);
#endif
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }

#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache_arr[n - 1];
#endif
}
