/*
 * A037366  Numbers whose base-6 and base-10 expansions have no digits in
 *          common.
 * https://oeis.org/A037366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose decimal digits and
 *            base‑6 digits are disjoint
 * Range:     u8 n <= 104, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return true if the number n has at least one digit that appears in both
 * its decimal and base‑6 representations. */
static bool has_common_digit(seq_t n)
{
    bool dec[10]  = {0};
    bool base6[10] = {0};
    seq_t tmp = n;

    /* Extract decimal digits. */
    if (tmp == 0) dec[0] = true;
    while (tmp > 0) {
        int d = (int)(tmp % (seq_t)10);
        dec[d] = true;
        tmp /= (seq_t)10;
    }

    /* Extract base‑6 digits. */
    tmp = n;
    if (tmp == 0) base6[0] = true;
    while (tmp > 0) {
        int d = (int)(tmp % (seq_t)6);
        base6[d] = true;
        tmp /= (seq_t)6;
    }

    /* Check for intersection. */
    for (int d = 0; d < 10; ++d)
        if (dec[d] && base6[d])
            return true;
    return false;
}

/* Return the nth term of the sequence (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but harmless */

    /* Static state: cache of computed terms, last tested integer, etc. */
    static idx_t last_checked  = 0;   /* last integer that was examined         */
    static idx_t num_terms     = 0;   /* number of terms currently cached       */
    static seq_t *cache        = NULL;/* array of cached terms                  */
    static size_t cache_cap    = 0;   /* capacity of the cache                  */

    /* Grow the cache as needed. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            abort();
    }

    /* Generate terms until we have at least n. */
    while (num_terms < n) {
        seq_t cand = (seq_t)(last_checked + 1);
        ++last_checked;

        if (!has_common_digit(cand)) {
            /* Resize cache if full. */
            if (num_terms == cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap == 0)
                    new_cap = 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache   = tmp;
                cache_cap = new_cap;
            }
            cache[num_terms++] = cand;
        }
    }

    return cache[n - 1];
}
