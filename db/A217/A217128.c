/*
 * A217128  Numbers n such that (2n)^4 + 1 is not prime.
 * https://oeis.org/A217128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A217128: Numbers n such that (2n)^4 + 1 is composite.  Computed by testing primality. */
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *a_cache   = NULL;
static size_t a_cache_cap = 0;
static size_t a_cache_len = 0;
static idx_t   a_tested_n  = 0;

static void a_grow_cache(void) {
    size_t new_cap = a_cache_cap == 0 ? 4 : a_cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    a_cache      = tmp;
    a_cache_cap  = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* out of range, but not expected */

    while (a_cache_len < n) {
        idx_t i   = a_tested_n + 1;
        idx_t val = (idx_t)16 * i * i * i * i + 1;   /* 16 * i^4 + 1 */
        if (!is_prime(val)) {
            if (a_cache_len == a_cache_cap) a_grow_cache();
            a_cache[a_cache_len++] = (seq_t)i;
        }
        a_tested_n = i;
    }
    return a_cache[n - 1];
}
