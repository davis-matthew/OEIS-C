/*
 * A037316  Numbers whose base-4 and base-5 expansions have the same digit
 *          sum.
 * https://oeis.org/A037316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 5528, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the n‑th positive integer whose base‑4 digit sum
 * equals its base‑5 digit sum.
 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of a(1), a(2), … */
static size_t cache_len = 0;  /* number of cached terms */
static size_t cache_cap = 0;  /* allocated capacity */

/* Ensure that the cache can hold at least one more element. */
static void ensure_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t newcap = (cache_cap == 0) ? 8 : cache_cap << 1;
        seq_t *p = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!p) abort();          /* out of memory */
        cache = p;
        cache_cap = newcap;
    }
}

/* Return the nth term (n ≥ 1).  For n == 0 the function returns 0
 * (the value is undefined for n < 1 but this is harmless).
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Start searching from the next integer after the last cached
         * value (or from 1 if no values are cached yet). */
        seq_t cand = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;
        seq_t x = cand;

        for (;;) {
            seq_t tmp;
            seq_t sum4 = 0;
            tmp = x;
            while (tmp) {
                sum4 += tmp % 4;
                tmp /= 4;
            }

            seq_t sum5 = 0;
            tmp = x;
            while (tmp) {
                sum5 += tmp % 5;
                tmp /= 5;
            }

            if (sum4 == sum5) {
                ensure_capacity();
                cache[cache_len++] = x;
                break;
            }
            ++x;
        }
    }

    return cache[n - 1];               /* zero‑based index in the array */
}
