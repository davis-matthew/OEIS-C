/*
 * A023989  Look and Say sequence: describe the previous term! (method C -
 *          initial term is 2).
 * https://oeis.org/A023989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   digit_frequencies, ensure_cache, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute: a(n) = A047842(a(n-1))  (method C)  with a(0) = 2 */

#include <stddef.h>
#include "../utils/seq_from_str.h"
#include "../utils/digit_frequencies.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static memoisation cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed_up_to = (idx_t)-1;   /* -1 means nothing computed yet */

    /* make sure there is space for indices up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* initialise the very first term once */
    if (computed_up_to == (idx_t)-1) {
        cache[0] = (seq_t)2;            /* direct assignment works for any seq_t */
        computed_up_to = 0;
    }

    /* generate all missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        /* compute freq of digits of the previous term */
        unsigned int freq[10];
        digit_frequencies(cache[i - 1], freq);

        /* build decimal representation of the next term */
        char buf[512];              /* ample space for any expected length */
        size_t pos = 0;

        for (int d = 0; d <= 9; ++d) {
            unsigned int cnt = freq[d];
            if (cnt == 0) continue;

            /* write count in decimal */
            char tmp[32];
            int tpos = 0;
            unsigned int tmp_cnt = cnt;
            do {
                tmp[tpos++] = '0' + (tmp_cnt % 10);
                tmp_cnt /= 10;
            } while (tmp_cnt);
            for (int k = tpos - 1; k >= 0; --k) {
                buf[pos++] = tmp[k];
            }

            /* write the digit itself */
            buf[pos++] = '0' + d;
            if (pos >= sizeof(buf) - 1) break;   /* safety guard */
        }
        buf[pos] = '\0';

        cache[i] = seq_from_str(buf);
    }
    computed_up_to = n;

    return cache[n];
}
