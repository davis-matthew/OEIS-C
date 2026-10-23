/*
 * A039538  Numbers whose base-12 representation has the same nonzero number
 *          of 5's and 6's.
 * https://oeis.org/A039538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑12 representation contains * the
 *            same nonzero number of digits 5 and 6 (both at least one)
 * Range:     u8 n <= 4, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t last_checked = 0;

seq_t a(idx_t n) {
    /* fast‑return if already computed */
    if (n == 0) return 0;               /* undefined for n=0, but safe */
    if (cache_len >= (size_t)n)
        return cache[n - 1];

    /* grow cache initially if needed */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        /* search next candidate */
        ++last_checked;
        seq_t x = (seq_t)last_checked;

        /* count 5's and 6's in base‑12 */
        int c5 = 0, c6 = 0;
        seq_t tmp = x;
        while (tmp != 0) {
            seq_t r = tmp % (seq_t)12;
            if (r == 5) ++c5;
            else if (r == 6) ++c6;
            tmp /= (seq_t)12;
        }

        /* check condition */
        if (c5 > 0 && c5 == c6) {
            /* ensure capacity for the new element */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = p;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
    }

    return cache[n - 1];
}
