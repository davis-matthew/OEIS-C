/*
 * A230918  Numbers k such that phi(k)^phi(k) == gcd(k, phi(k)) (mod k), where
 *          phi is the Euler totient function.
 * https://oeis.org/A230918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that phi(k)^phi(k) == gcd(k,phi(k)) (mod k) */
/* Implemented by a simple brute‑force search with memoisation. */
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t next_k = 1;

static void cache_push(idx_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) abort();
        cache = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)val;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (cache_len < n) {
        idx_t k = next_k++;
        if (k == 1) {            /* 1 is a trivial solution */
            cache_push(1);
            continue;
        }
        seq_t phi_k = phi(k);
        unsigned long long phi64 = (unsigned long long)phi_k;   /* phi(k) fits in 64‑bit up to our range */
        seq_t pow_v = mod_pow(phi_k, phi64, (seq_t)k);
        seq_t g = gcd((seq_t)k, phi_k);
        if (pow_v == g) cache_push(k);
    }
    return cache[n - 1];
}
