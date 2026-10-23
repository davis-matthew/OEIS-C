/*
 * A186400  Numbers k such that there are 8 primes between 100*k and 100*k +
 *          99.
 * https://oeis.org/A186400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k for which the interval [100*k,
 *            100*k+99] contains exactly eight primes. The implementation
 *            builds a prime sieve up to a fixed limit and scans the planet in
 *            100‑number centuries, storing those k whose prime count equals
 *            eight. Cache is built lazily and reused for any ord...
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*--- Parameters --------------------------------------------------------*/
#define SIEVE_LIMIT      ((idx_t)200000000U)   /* 200 million, ample for 10 000 terms */

/*--- Static storage ---------------------------------------------------*/
static bool        sieve_ready = false;   /* true after bitset is built     */
static uint8_t    *sieve_bits = NULL;     /* bitset for 0..SIEVE_LIMIT      */
static uint64_t   *cache      = NULL;     /* cached k values (uint64_t)    */
static size_t      cache_len  = 0;        /* number of cached values       */
static size_t      cache_cap  = 0;        /* allocated capacity            */
static idx_t      current_block = 0;      /* next 100‑block to process     */

/*--- Bit operations ---------------------------------------------------*/
static inline bool  test_bit(idx_t i)  { return (sieve_bits[i >> 3] >> (i & 7U)) & 1U; }
static inline void  clear_bit(idx_t i) { sieve_bits[i >> 3] &= (uint8_t)(~(1U << (i & 7U))); }

/*--- Sieve of Eratosthenes -------------------------------------------*/
static void build_sieve(void)
{
    if (sieve_ready) return;

    idx_t limit = SIEVE_LIMIT;
    size_t bytes = (size_t)(limit >> 3) + 1;
    sieve_bits = (uint8_t *)malloc(bytes);
    if (!sieve_bits) abort();

    /* Start with all bits set and clear 0 and 1. */
    memset(sieve_bits, 0xFF, bytes);
    clear_bit(0);
    clear_bit(1);

    /* Eratosthenes: clear composites. */
    for (idx_t p = 2; p * p <= limit; ++p)
        if (test_bit(p))
            for (idx_t x = p * p; x <= limit; x += p)
                clear_bit(x);

    sieve_ready = true;
}

/*--- Cache helper ------------------------------------------------------*/
static void push_cache(uint64_t k)
{
    if (cache_len == cache_cap) {
        size_t newcap = (cache_cap == 0) ? 32 : cache_cap << 1;
        uint64_t *newp = (uint64_t *)realloc(cache, newcap * sizeof(uint64_t));
        if (!newp) abort();
        cache = newp;
        cache_cap = newcap;
    }
    cache[cache_len++] = k;
}

/*--- Main sequence function ------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */

    /* Lazily initialize sieve and cache. */
    build_sieve();
    if (cache == NULL) { cache = NULL; cache_cap = 0; cache_len = 0; }

    /* Generate terms until we have at least n of them. */
    const idx_t max_blocks = SIEVE_LIMIT / 100U;
    while (cache_len < n && current_block < max_blocks) {
        idx_t start = current_block * 100U;
        idx_t end   = start + 99U;
        idx_t count = 0;

        for (idx_t x = start; x <= end; ++x)
            if (test_bit(x)) ++count;

        if (count == 8)
            push_cache((uint64_t)current_block);

        ++current_block;
    }

    /* If we ran out of limit, we would need to extend the sieve.
       For the expected use (n ≤ 10 000) the chosen limit is sufficient. */
    if (cache_len < n) abort();

    return (seq_t)cache[n - 1];
}
