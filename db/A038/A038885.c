/*
 * A038885  14 is a square mod p.
 * https://oeis.org/A038885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th prime p for which 14 is a quadratic residue
 *            modulo p (primes that divide 14, i.e., p = 2 or 7, are
 *            included). The sequence starts with a(1) = 2, a(2) = 5, a(3) =
 *            7, a(4) = 11, …
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps a cache of computed values and extends it as needed.
   The prime test uses the supplied `is_prime` helper, and Legendre symbols are
   evaluated with `mod_pow` (Euler's criterion).  The algorithm runs in O(n log n)
   time, which comfortably satisfies the 2 s performance requirement for n up
   to about 10 000. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

typedef seq_t seq_t;
typedef idx_t idx_t;

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t current_checked = 2;   /* next integer to test for primality */

static bool ok_for_prime(idx_t p)
{
    if (p == 2 || p == 7)
        return true;  /* 14 ≡ 0 (mod p) → 0² = 0, hence residue */

    /* Euler's criterion: (14|p) ≡ 14^((p-1)/2) (mod p) */
    seq_t r = mod_pow((seq_t)14, (p - 1) >> 1, (seq_t)p);
    return (r == (seq_t)1) || (r == (seq_t)0);  /* only r==1 for odd primes */
}

static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < n) {
        /* Test current integer for primeness */
        if (is_prime(current_checked)) {
            if (ok_for_prime(current_checked)) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)current_checked;
            }
        }
        current_checked++;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for non‑positive indices */
    ensure_cache(n);
    return cache[n - 1];  /* sequence offset of 1 */
}
