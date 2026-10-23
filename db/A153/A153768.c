/*
 * A153768  Numbers n such that n, 8n-9 and 8n+9 are primes.
 * https://oeis.org/A153768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 272, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A153768: numbers n such that n, 8n‑9 and 8n + 9 are primes.
 * The function checks successive integers, verifies the three
 * primality conditions using a growing bit‑sieve, and caches all
 * discovered terms for fast repeated queries. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --- internal data ---------------------------------------------- */

static uint8_t *prime_bits = NULL;      /* bitset for odd numbers only   */
static idx_t   sieve_limit = 0;         /* maximum value covered by prime_bits */
static size_t  cache_cap   = 0;         /* capacity of the cache array   */
static size_t  cache_len   = 0;         /* number of terms stored        */
static seq_t  *cache       = NULL;      /* cached sequence values        */
static idx_t   last_n_scanned = 1;      /* last n that was examined      */
static bool    initialized = false;     /* whether init performed        */

/* bit operations on odd numbers: index 0 → 3 */
#define BIT_SET(arr,i)   ((arr)[(i)>>3] & (1u << ((i)&7)))
#define BIT_CLEAR(arr,i) ((arr)[(i)>>3] &= ~(1u << ((i)&7)))

/* Build a sieve up to 'limit' (inclusive).  Only odd numbers ≥3 are stored. */
static void build_sieve(idx_t limit)
{
    idx_t odd_cnt = 0;
    if (limit >= 3)
        odd_cnt = (limit - 3) / 2 + 1;

    size_t byte_len = (odd_cnt + 7) / 8;
    uint8_t *new_bits = (uint8_t *)calloc(byte_len, 1);
    if (!new_bits) exit(1);

    /* Set all odd numbers to prime initially. */
    for (size_t i = 0; i < byte_len; ++i)
        new_bits[i] = 0xFFu;

    /* Determine largest odd p ≤ sqrt(limit). */
    idx_t sqrt_lim = 0;
    for (idx_t p = 3; (p * p) <= limit; p += 2)
        sqrt_lim = p;

    /* Sieve of Eratosthenes for odd numbers. */
    idx_t odd_cnt_local = odd_cnt;
    for (idx_t i = 0; ; ++i) {
        idx_t p = 3 + 2 * i;
        if (p > sqrt_lim) break;
        if (!BIT_SET(new_bits, i)) continue;  /* composite */

        idx_t start = (p * p - 3) / 2;             /* index of p*p */
        for (idx_t j = start; j < odd_cnt_local; j += p)
            BIT_CLEAR(new_bits, j);
    }

    if (prime_bits) free(prime_bits);
    prime_bits   = new_bits;
    sieve_limit  = limit;
}

/* Ensure the sieve covers at least up to 8*n+9. */
static void ensure_sieve_limit(idx_t n)
{
    idx_t need = 8 * n + 9;
    if (need <= sieve_limit) return;

    idx_t new_limit = sieve_limit ? (sieve_limit << 1) : need;
    if (new_limit < need) new_limit = need;
    build_sieve(new_limit);
}

/* Test primality for an idx_t using the bit sieve. */
static bool is_prime_fast(idx_t num)
{
    if (num == 2) return true;
    if (num < 2 || ((num & 1u) == 0)) return false;
    if (num > sieve_limit) return false;           /* safety */
    idx_t idx = (num - 3) >> 1;                    /* index of odd number */
    return BIT_SET(prime_bits, idx);
}

/* Expand the cache until it contains at least 'target' terms. */
static void fill_cache_up_to(idx_t target)
{
    if (!initialized) {
        /* Basic initialization: small sieve, modest cache. */
        idx_t init_n = 100000;                     /* covers many early terms */
        build_sieve(8 * init_n + 9);
        cache_cap = 4096;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) exit(1);
        cache_len = 0;
        last_n_scanned = 1;
        initialized = true;
    }

    for (idx_t n = last_n_scanned + 1; ; ++n) {
        ensure_sieve_limit(n);
        if (is_prime_fast(n) && is_prime_fast(8 * n - 9) && is_prime_fast(8 * n + 9)) {
            /* store the term */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!cache) exit(1);
            }
            cache[cache_len++] = (seq_t)n;
            if (cache_len >= target) {
                last_n_scanned = n;
                return;
            }
        }
        last_n_scanned = n;
    }
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* out of bounds */
    fill_cache_up_to(n);
    return cache[n - 1];
}
