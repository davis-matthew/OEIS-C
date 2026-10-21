/*
 * A090614  Numbers n such that 14n+3 is prime.
 * https://oeis.org/A090614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k for which 14*k+3 is prime (offset 1)
 * Range:     u8 n <= 11, u16 n <= 961, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache      = NULL;   /* storage for a(1)…a(cache_len) */
static size_t  cache_cap = 0;      /* allocated array capacity */
static idx_t   cache_len = 0;      /* number of computed terms */
static idx_t   next_m    = 0;      /* next candidate m to test */

/* Return a(n) (1‑based).  Undefined for n==0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* safety for out‑of‑range */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t val = ((seq_t)14 * (seq_t)next_m) + 3;
        if (is_prime_big(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }

    return cache[n - 1];
}
