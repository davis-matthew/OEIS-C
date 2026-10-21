/*
 * A213632  Primes p such that primality of (p+p')/2+4 and of (p+p')/2-4
 *          differ, where p'=prevprime(p-1), the next smaller prime.
 * https://oeis.org/A213632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   append_cache, build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A213632: primes p such that primality of (p+p')/2+4 and of (p+p')/2-4 differ. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_cache.h"

/* Cached state. */
static uint64_t *primes     = NULL;   /* ascending list of primes */
static size_t   primes_cnt  = 0;
static size_t   proc_idx    = 1;      /* next prime index to examine */

static seq_t *cache        = NULL;   /* sequence values found so far */
static size_t cache_len    = 0;
static size_t cache_cap    = 0;

/* One‑time initialization. */
static void init(void) {
    if (primes != NULL) return;
    const idx_t LIMIT = 20000000;            /* plenty for n up to ~10 000 */
    build_primes_upto(LIMIT, &primes, &primes_cnt);
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined; safe fallback */

    init();

    /* Generate terms until we have enough. */
    while (cache_len < n) {
        if (proc_idx >= primes_cnt) break;   /* unlikely for requested n */

        uint64_t p    = primes[proc_idx];
        uint64_t prev = primes[proc_idx - 1];

        seq_t m = ((seq_t)(p + prev)) / 2;     /* average of two consecutive primes */
        bool b1 = is_prime_big(m + 4);
        bool b2 = is_prime_big(m - 4);

        if (b1 != b2) {
            append_cache(&cache, &cache_len, &cache_cap, (seq_t)p);
        }
        ++proc_idx;
    }

    if (cache_len < n) return (seq_t)0;  /* graceful fallback if limit insufficient */
    return cache[n - 1];
}
