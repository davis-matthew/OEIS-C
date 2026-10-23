/*
 * A014528  Neither == 0 (mod 4) nor a power of 3.
 * https://oeis.org/A014528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014528: numbers that are neither 0 (mod 4) nor a power of 3 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return true if x is a power of 3 with exponent ≥ 1 (1 itself is not treated as a power). */
static bool is_power_of_3(seq_t x)
{
    if (x == (seq_t)1) return false;          /* 1 is allowed in the sequence */
    if (x % (seq_t)3 != (seq_t)0) return false;
    while (x % (seq_t)3 == (seq_t)0) x = x / (seq_t)3;
    return x == (seq_t)1;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* cached terms */
    static size_t cap   = 0;     /* current allocation size */
    static size_t len   = 0;     /* number of cached terms */

    /* Initialise cache on first call. */
    if (cap == 0) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n + 1 cached values. */
    for (; len <= n; ) {
        seq_t cand = (len == 0) ? (seq_t)1 : cache[len - 1] + (seq_t)1;
        for (;;) {
            if (cand % (seq_t)4 != (seq_t)0 && !is_power_of_3(cand)) {
                if (len == cap) {                 /* grow capacity when needed */
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = cand;
                if (len > n) return cache[(size_t)n];
                break;                           /* move on to next candidate */
            }
            cand = cand + (seq_t)1;
        }
    }

    /* This point is reached only when a previously cached value is requested. */
    return cache[(size_t)n];
}
