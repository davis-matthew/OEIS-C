/*
 * A124924  Primes p such that p^2 divides A124923((3p-1)/2) =
 *          ((3p-1)/2)^(3(p-1)/2) + 1.
 * https://oeis.org/A124924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = n-th prime p such that p^2 divides ((3p-1)/2)^((3p-1)/2)+1 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper prototypes – the build supplies the actual implementations. */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */
#include "../utils/mod_pow.h"           /* seq_t mod_pow(seq_t, uint64_t, seq_t) */

#ifdef __cplusplus
#include <vector>
#endif

/* static storage for the sequence cache */
static idx_t next_start = 3;   /* first candidate to test, odd numbers only */

#ifdef __cplusplus
static std::vector<seq_t> cache;
static void grow_cache(size_t needed) { /* std::vector auto‑resizes */ }
#else
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static void grow_cache(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}
#endif

/* test whether candidate prime p satisfies the A124924 condition */
static bool test_prime(idx_t p) {
    /* a = (p-1)/2 must be a quadratic non‑residue mod p */
    idx_t a = (p - 1) >> 1;
    seq_t p64 = (seq_t)p;
    seq_t a64 = (seq_t)a;

    seq_t leg = mod_pow(a64, (uint64_t)a, p64);
    if (leg != p64 - 1) return false;  /* not a QNR */

    /* Compute a^a mod p^2 */
    seq_t modp2 = p64 * p64;
    seq_t a_pow = mod_pow(a64, (uint64_t)a, modp2);

    /* u = ((a^a + 1)/p) mod p; must equal 1 */
    seq_t u = (a_pow + 1) / p64;      /* result < p */
    if (u % p64 != 1) return false;

    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* out of bounds */

    /* Ensure the cache contains at least n elements */
    while (
#ifdef __cplusplus
           cache.size() < n
#else
           cache_len < n
#endif
           ) {
        /* search for the next qualifying prime */
        for (idx_t cand = next_start; ; cand += 2) {
            if (!is_prime(cand)) continue;
            next_start = cand + 2;  /* move start for next search */

            if (test_prime(cand)) {
#ifdef __cplusplus
                cache.push_back(cand);
#else
                grow_cache(cache_len + 1);
                cache[cache_len++] = cand;
#endif
                break;
            }
        }
    }

#ifdef __cplusplus
    return cache[n - 1];
#else
    return cache[n - 1];
#endif
}
