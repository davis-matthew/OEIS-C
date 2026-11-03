/*
 * A083491  Multiples of 3 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is defined by: a(0) = 3.  For n ≥ 1,
   a(n) is the smallest multiple of 3 larger than a(n-1) that shares
   no decimal digit with a(n-1).  The function below computes the
   terms lazily and caches previously found values. */
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: return 1 if x and y share a decimal digit, 0 otherwise. */
static int have_common_digit(seq_t x, seq_t y)
{
    unsigned int mask = 0;
    while (x) {
        unsigned int d = (unsigned int)(x % (seq_t)10);
        mask |= 1u << d;
        x /= (seq_t)10;
    }
    while (y) {
        unsigned int d = (unsigned int)(y % (seq_t)10);
        if (mask & (1u << d)) return 1;
        y /= (seq_t)10;
    }
    return 0;
}

/* Sequence entry */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;      /* array of terms */
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t cache_cap = 0;      /* allocated size */

    /* Ensure capacity for at least n+1 terms. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_cap);
        if (!tmp) return 0;            /* allocation failed – unlikely in tests */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first term if cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;
        cache_len = 1;
    }

    /* Generate terms until we have at least n+1 entries. */
    while (cache_len <= n) {
        seq_t last = cache[cache_len - 1];
        seq_t candidate = last + (seq_t)3;

        /* Skip candidates that share a digit with the last term. */
        while (have_common_digit(last, candidate))
            candidate += (seq_t)3;

        cache[cache_len++] = candidate;
    }

    return cache[n];
}
