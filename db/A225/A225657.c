/*
 * A225657  Union of {6} and A000961.
 * https://oeis.org/A225657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth term of the sorted union of {6} and all prime powers
 *            (including 1)
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   add_value, generate_prime_powers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/add_value.h"
#include "../utils/generate_prime_powers.h"

/* comparison function for qsort on seq_t values */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *v1 = (const seq_t *)p1;
    const seq_t *v2 = (const seq_t *)p2;
    if (*v1 < *v2) return -1;
    if (*v1 > *v2) return 1;
    return 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static idx_t   cache_lim = 0;   /* sieve limit we have generated so far */

    /* ensure we have at least n terms cached */
    while (cache_len < n) {
        /* double the sieve limit until the cache covers n */
        idx_t new_lim = (cache_lim == 0) ? 1024 : cache_lim * 2;
        /* regenerate cache */
        seq_t *vals = NULL;
        size_t vals_len = 0, vals_cap = 0;
        generate_prime_powers(new_lim, &vals, &vals_len, &vals_cap);

        /* sort and deduplicate */
        qsort(vals, vals_len, sizeof(seq_t), cmp_seq);
        size_t uniq = 0;
        for (size_t i = 0; i < vals_len; ++i) {
            if (uniq == 0 || vals[i] != vals[uniq - 1]) {
                vals[uniq++] = vals[i];
            }
        }
        vals_len = uniq;

        /* free previous cache */
        free(cache);
        cache = vals;
        cache_len = vals_len;
        cache_lim = new_lim;

        /* safety: if after regeneration we still don't have enough terms, increase limit */
        if (cache_len < n) {
            /* increase limit further */
            continue;
        }
    }

    /* return the (n‑1)th element (1‑based indexing) */
    return cache[n - 1];
}
