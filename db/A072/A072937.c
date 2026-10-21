/*
 * A072937  Least k such that prime(n) appears in factorization of k! + 1.
 * https://oeis.org/A072937
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest k such that prime(n) divides k! + 1 (i.e. k! ≡
 *            –1 (mod prime(n))), n ≥ 2
 * Range:     u8 n <= 59, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* get_prime(idx) returns the idx‑th prime (1‑based)  */
#include "../utils/get_prime.h"

/* dynamic array helpers for arbitrary‑precision seq_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* cache[n] holds a(n) for n ≥ 0 */
static size_t cache_cap = 0;     /* allocated number of elements */
static idx_t  cache_upto = 1;    /* highest n for which a(n) is computed */

/* enlarge the cache up to at least n (n ≥ 2) */
static void ensure_cache(idx_t n)
{
    if (n < 2 || n <= cache_upto)
        return;

    /* size needed for indices 0 … n (inclusive) */
    size_t needed = (size_t)n + 1;

    if (cache == NULL) {
        cache = seq_alloc(needed);
    } else if (needed > cache_cap) {
        cache = seq_realloc(cache, cache_cap, needed);
    }
    cache_cap = needed;

    /* compute new terms */
    for (idx_t idx = cache_upto + 1; idx <= n; ++idx) {
        uint64_t p = get_prime(idx);     /* prime(n) */
        uint64_t f = 1;                  /* running product modulo p */
        uint64_t k;
        for (k = 1; k < p; ++k) {
            f = (f * k) % p;
            if (f == p - 1)
                break;
        }
        cache[idx] = (seq_t)k;
    }
    cache_upto = n;
}

/* public entry point required by the tester framework */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* undefined for n < 2 */

    ensure_cache(n);
    return cache[n];
}
