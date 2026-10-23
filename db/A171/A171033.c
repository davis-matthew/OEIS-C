/*
 * A171033  In the sequence of prime numbers, replace all the '6' digits with
 *          '5' and vice versa.
 * https://oeis.org/A171033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   swap digits 5↔6 of the n‑th prime (n ≥ 1)
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, prime_limit_for_idx, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc_array.h"

/* Convert decimal digits: 5 ↔ 6 */
static seq_t swap_digits(seq_t v)
{
    seq_t res = 0;
    seq_t mul  = 1;
    while (v > 0) {
        uint64_t d = (uint64_t)(v % 10);
        if (d == 5) d = 6;
        else if (d == 6) d = 5;
        res += (seq_t)d * mul;
        mul  *= 10;
        v    /= 10;
    }
    return res;
}

/* Cached prime table (0‑indexed) */
static seq_t *prime_cache = NULL;
static idx_t  prime_cache_max = 0;   /* number of primes stored */

/* Ensure the cache holds at least the first n primes */
static void ensure_cache(idx_t n)
{
    if (n <= prime_cache_max) return;

    /* Estimate an upper bound for the n‑th prime */
    idx_t limit = prime_limit_for_idx(n);

    uint64_t *primes64 = NULL;
    size_t    cnt = 0;
    build_primes_upto(limit, &primes64, &cnt);

    /* Allocate a new cache in seq_t type */
    seq_t *new_cache = seq_alloc_array(cnt);
    for (size_t i = 0; i < cnt; ++i) {
        new_cache[i] = (seq_t)primes64[i];
    }

    /* Replace old cache (if any) */
    free(prime_cache);
    prime_cache = new_cache;
    prime_cache_max = cnt;

    free(primes64);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined below offset */
    ensure_cache(n);               /* now prime_cache has at least n primes */
    seq_t p = prime_cache[n - 1];  /* primes are 0‑indexed in the cache */
    return swap_digits(p);
}
