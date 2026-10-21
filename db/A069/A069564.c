/*
 * A069564  a(1) = 2; a(n) = k*a(n-1) + 1 is a multiple of n-th prime with k >
 *          1.
 * https://oeis.org/A069564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1) = 2; for n > 1, a(n) = k * a(n-1) + 1 where k > 1 is the
 *            smallest integer such that a(n) is divisible by the nth prime
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   ensure_cache, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for index 0 */

    static seq_t *cache = NULL;      /* cached terms, 1‑indexed */
    static size_t cache_cap = 0;     /* allocated size (in seq_t units) */
    static idx_t cached_n = 0;       /* number of terms stored (>= 0) */

    /* Make sure the cache can hold at least n terms */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise first term if this is the first call */
    if (cached_n == 0) {
        cache[0] = (seq_t)2;         /* a(1) = 2 */
        cached_n   = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];   /* a(i‑1) */

        /* Failure propagate: if previous term is already 0, keep 0 */
        if (prev == 0) {
            cache[i - 1] = (seq_t)0;
            continue;
        }

        uint32_t p = get_prime(i);   /* ith prime (i‑indexed, 1 → 2) */

        seq_t rem = prev % (seq_t)p; /* remainder of a(i‑1) mod p */

        /* If a(i‑1) is already divisible by p, no suitable k exists */
        if (rem == 0) {
            cache[i - 1] = (seq_t)0;
            continue;
        }

        /* Compute the modular inverse of rem modulo p */
        seq_t inv = mod_pow(rem, (idx_t)p - 2, (seq_t)p);

        /* Minimal solution of k*rem ≡ p−1 (mod p) */
        seq_t k0 = ((seq_t)(p - 1) * inv) % (seq_t)p;
        seq_t k  = k0;
        if (k0 <= (seq_t)1)           /* need k > 1 */
            k += (seq_t)p;

        /* a(i) = k * a(i‑1) + 1 */
        cache[i - 1] = k * prev + (seq_t)1;
    }

    cached_n = n;
    return cache[n - 1];
}
