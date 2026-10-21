/*
 * A229786  a(n) = n-th prime modulo 23.
 * https://oeis.org/A229786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Prime(n) mod 23 Computes the n‑th prime (n≥1) and returns
 *            its remainder modulo 23. The function caches primes so that each
 *            n can be answered in amortised O(1) time
 * Range:     u8 n <= 54, u16 n <= 1900, u32 n <= 1900, u64 n <= 1900,
 *            u128 n <= 1900, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assume the following helpers are available:
   - uint64_t* build_primes_upto(idx_t limit, uint64_t **out, size_t *cnt)
   - idx_t _prime_limit_for_idx(idx_t n)
   They are declared in the corresponding header files. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache = NULL;  /* array of the first primes */
static size_t prime_cache_cnt = 0;     /* number of primes cached */

/* Ensure that prime_cache contains at least n primes. */
static void ensure_primes(idx_t n)
{
    if (n == 0) return;
    if (n <= prime_cache_cnt) return;  /* already have enough */

    /* Determine a bound that guarantees at least n primes. */
    idx_t limit = _prime_limit_for_idx(n);

    /* Allocate a new array of primes up to that limit. */
    uint64_t *new_cache = NULL;
    size_t new_cnt = 0;
    build_primes_upto(limit, &new_cache, &new_cnt);

    /* The library guarantees new_cnt >= n. */
    /* Replace old cache. */
    free(prime_cache);
    prime_cache = new_cache;
    prime_cache_cnt = new_cnt;
}

/* External function requested by the requirements. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not used but graceful */
    ensure_primes(n);
    /* n-1 is the zero‑based index into prime_cache. */
    seq_t mod = (seq_t)23;
    seq_t val = (seq_t)prime_cache[n - 1];
    return val % mod;
}
