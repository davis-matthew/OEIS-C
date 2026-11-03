/*
 * A097934  Primes p that divide 3^((p-1)/2) - 2^((p-1)/2).
 * https://oeis.org/A097934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 947 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primes p satisfying 3^((p-1)/2) ≡ 2^((p-1)/2) (mod p) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Return the nth (1‑based) prime with the above property. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 */

    static seq_t *cache   = NULL;         /* cached qualifying primes   */
    static size_t cache_cap = 0, cache_len = 0;

    static idx_t candidate = 5;           /* start at the first odd > 3   */

    /* enlarge the cache when needed */
    while (cache_len < (size_t)n) {
        /* find the next candidate prime */
        if (!is_prime(candidate)) {
            candidate += 2;                /* only odd numbers are needed */
            continue;
        }

        /* exponent (p-1)/2 fits in idx_t */
        idx_t exp = (candidate - 1) >> 1;
        seq_t mod = (seq_t)candidate;

        /* compare 3^exp and 2^exp modulo p */
        if (powmod((seq_t)3, exp, mod) == powmod((seq_t)2, exp, mod)) {
            /* store the prime in the cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(1);           /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = mod;
        }

        candidate += 2;                    /* next odd number */
    }

    return cache[n - 1];
}
