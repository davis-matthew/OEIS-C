/*
 * A067503  Powers of 6 with digit sum also a power of 6.
 * https://oeis.org/A067503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   append_candidate, digit_sum, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A067503: Powers of 6 whose digit sum is also a power of 6.
   Algorithm: generate successive powers of 6, test whether the
   decimal digit sum is a pure power of 6, and keep those powers in a
   static cache.  The cache grows on-demand, so calls may arrive in
   any order. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mul_small.h"
#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"

static seq_t *cache = NULL;          /* cached terms of the sequence      */
static size_t cache_len = 0;         /* number of cached terms            */
static size_t cache_cap = 0;         /* current capacity of cache         */
static seq_t cur = 0;                /* current power of 6 (6^current_exp) */
static idx_t  current_exp = 0;       /* exponent corresponding to cur      */

/* Return the n‑th term, 1‑based.  On the first call the cache is
   initialised with a(1)=1.  The cache is extended until it contains
   at least n terms. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for offset 1 – return 0 as a safe default. */
        return 0;
    }

    /* Initialise cache on first use. */
    if (cache_len == 0) {
        cur = 1;                 /* 6^0 */
        current_exp = 0;
        append_candidate(&cache, &cache_len, &cache_cap, cur);
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* Move to the next power of 6. */
        current_exp++;
        cur = mul_small(cur, 6);

        /* Digit sum of the current power. */
        seq_t s = digit_sum(cur);
        seq_t t = s;
        if (t == 0) {
            /* Digit sum cannot be a power of 6. */
        } else {
            /* Reduce t by divisions by 6; if it ends at 1,
               it was a power of 6. */
            while (t % 6 == 0) {
                t /= 6;
            }
            if (t == 1) {
                append_candidate(&cache, &cache_len, &cache_cap, cur);
            }
        }
    }

    return cache[n - 1];
}
