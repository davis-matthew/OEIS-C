/*
 * A096099  a(0) = 1, a(n) = least number such that the concatenation of all
 *          terms through a(n) is divisible by prime(n).
 * https://oeis.org/A096099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the least integer m ≥ 1 such that the concatenation
 *            a(0)...a(n) is divisible by the n‑th prime (prime(0)=2). The
 *            following implementation computes a(n) on demand with
 *            memoisation
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_cache, digit_len, free_cache, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/digit_len.h"
#include "../utils/mod_pow.h"
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

/* Cache of computed terms.  cache[i]==0 indicates "not yet computed". */
static seq_t *cache   = NULL;
static size_t cache_len = 0;

/* Allocate (or enlarge) the cache to hold indices 0..n.  All new
   entries are initialised to 0. */
static void ensure_cache(size_t n)
{
    if (cache_len > n) return;

    size_t new_len = (cache_len == 0) ? 1 : cache_len;
    while (new_len <= n) new_len <<= 1;

    /* Allocate new array */
    seq_t *new_cache = alloc_cache((idx_t)new_len);
    /* Initialise new slots to zero */
    for (size_t i = 0; i < new_len; ++i) new_cache[i] = 0;

    /* Copy old values and free the old cache */
    if (cache) {
        size_t copy_len = (cache_len < new_len) ? cache_len : new_len;
        for (size_t i = 0; i < copy_len; ++i) new_cache[i] = cache[i];
        free_cache(cache);
    }

    cache   = new_cache;
    cache_len = new_len;
}

/* Recursive helper that guarantees that cache[n] is filled. */
static seq_t compute_a(idx_t n)
{
    if (n == 0) {               /* base case */
        cache[0] = 1;
        return 1;
    }

    if (cache[n] != 0) return cache[n];     /* already computed */

    /* Ensure all previous terms are available */
    compute_a(n - 1);

    /* Prime for this index */
    uint32_t p32 = get_prime(n);
    seq_t  p    = (seq_t)p32;                /* cast to seq_t */

    /* R = remainder of concatenation a(0)...a(n-1) modulo p */
    seq_t R = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t ai = cache[i];
        int    di = digit_len(ai);               /* decimal digits of ai */
        seq_t pow10 = mod_pow((seq_t)10, (idx_t)di, p);
        seq_t ai_mod = ai % p;
        R = (R * pow10 + ai_mod) % p;
    }

    /* Find the smallest k ≥ 1 with (R * 10^len(k) + k) ≡ 0 (mod p) */
    seq_t k = 1;
    for (;;) {
        int dk = digit_len(k);
        seq_t pow10k = mod_pow((seq_t)10, (idx_t)dk, p);
        seq_t test = (R * pow10k + k) % p;
        if (test == 0) break;
        ++k;
    }

    cache[n] = k;
    return k;
}

/* Public API */
seq_t a(idx_t n)
{
    ensure_cache((size_t)n);
    return compute_a(n);
}
