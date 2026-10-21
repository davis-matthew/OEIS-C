/*
 * A200817  Primes of the form (2^n - n)*2^n + 1.
 * https://oeis.org/A200817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A200817: Primes of the form (2^m - m)*2^m + 1. */
/* The function returns the nth such prime (n ≥ 1). */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* static cache for computed terms */
static seq_t *prime_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t       m_index = 0;  /* current exponent m */
static seq_t pow2 = (seq_t)1;    /* current value of 2^m */

/* return a(n) */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_len == 0) {
        cache_cap   = 16;
        prime_cache = seq_alloc(cache_cap);
        m_index     = 0;
        pow2        = (seq_t)1;
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* candidate value: (2^m - m)*2^m + 1 */
        seq_t tmp       = pow2 - (seq_t)m_index;   /* 2^m - m */
        seq_t candidate = tmp * pow2;              /* (2^m - m)*2^m */
        candidate = candidate + (seq_t)1;          /* +1 */

        if (is_prime_big(candidate)) {
            /* grow cache if necessary */
            if (cache_len + 1 > cache_cap) {
                size_t old_cap = cache_cap;
                cache_cap      = cache_cap * 2;
                prime_cache    = seq_realloc(prime_cache, old_cap, cache_cap);
            }
            prime_cache[cache_len++] = candidate;
        }

        /* advance to next exponent */
        m_index++;
        pow2 <<= 1;   /* 2^m = 2 * 2^(m-1) */
    }

    return prime_cache[n - 1];
}
