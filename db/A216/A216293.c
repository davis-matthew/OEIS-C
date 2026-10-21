/*
 * A216293  Values of k such that there are exactly two primes between 10k and
 *          10k + 9.
 * https://oeis.org/A216293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 9957, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A216293: Calculate the n-th k such that exactly two primes lie in
   the interval [10k, 10k+9].  The function uses an incremental sieve
   to test primality efficiently.  It works for any seq_t and idx_t
   provided by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  SIEVE:  Maintain a simple boolean array of primes up to a limit.
          The array is reallocated when a larger limit is needed.  */
static uint8_t *prime_flag = NULL;     /* 1 = prime, 0 = composite */
static uint32_t sieve_limit = 0;       /* maximum number sieved so far */

/* Build or rebuild the sieve up to 'limit' (inclusive). */
static void build_sieve(uint32_t limit)
{
    /* Allocate new array */
    uint8_t *new_flag = (uint8_t *)malloc((size_t)(limit + 1));
    if (!new_flag) return;                      /* safety */
    memset(new_flag, 1, (size_t)(limit + 1));
    new_flag[0] = new_flag[1] = 0;

    /* Classic Eratosthenes */
    uint32_t i, j;
    for (i = 2; (uint64_t)i * i <= limit; ++i) {
        if (new_flag[i]) {
            for (j = i * i; j <= limit; j += i)
                new_flag[j] = 0;
        }
    }

    /* Replace old array */
    free(prime_flag);
    prime_flag = new_flag;
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/*  CACHE:  Store already found k values.  */
static seq_t *k_cache = NULL;     /* array of k values */
static size_t cache_len = 0;      /* number of values stored */
static size_t cache_cap = 0;      /* allocated capacity */

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
    if (!new_arr) return;                 /* safety */
    k_cache = new_arr;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  MAIN FUNCTION:  Return the n-th term (1‑based).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */

    static uint64_t last_k = 1;          /* start checking from k=1 */

    /* Grow the cache until we have at least n elements. */
    while (cache_len < (size_t)n) {
        /* Ensure the sieve covers the current interval. */
        uint64_t interval_end = 10 * last_k + 9;
        if (interval_end > sieve_limit) {
            /* Double the sieve limit until it covers the interval. */
            uint32_t new_limit = sieve_limit ? sieve_limit * 2 : 1000000;
            while ((uint64_t)new_limit < interval_end) new_limit *= 2;
            build_sieve(new_limit);
        }

        /* Count primes in [10k,10k+9]. */
        unsigned prime_cnt = 0;
        uint64_t start = 10 * last_k;
        for (uint64_t i = 0; i < 10 && prime_cnt <= 2; ++i) {
            uint64_t val = start + i;
            if (prime_flag[(size_t)val]) ++prime_cnt;
        }

        if (prime_cnt == 2) {
            ensure_cache_capacity(cache_len + 1);
            k_cache[cache_len++] = (seq_t)last_k;
        }

        ++last_k;
    }

    return k_cache[(size_t)n - 1];
}
