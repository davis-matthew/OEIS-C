/*
 * A038904  Primes p such that 30 is not a square mod p.
 * https://oeis.org/A038904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p such that 30 is not a quadratic residue
 *            modulo p (i.e. 30^((p‑1)/2) ≡ p‑1 (mod p))
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *cache  = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;
static idx_t  last_checked = 6;          /* start testing from prime 7 */

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    idx_t p = last_checked + 1;
    while (cache_cnt < n) {
        if (is_prime(p)) {
            seq_t rem = mod_pow((seq_t)30, (p - 1) / 2, (seq_t)p);
            if (rem == (seq_t)(p - 1)) {
                if (cache_cnt == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) exit(1);
                }
                cache[cache_cnt++] = (seq_t)p;
            }
        }
        last_checked = p;
        ++p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n - 1];
}
