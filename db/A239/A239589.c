/*
 * A239589  Numbers whose squares are cyclops numbers.
 * https://oeis.org/A239589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer whose square is a cyclops number (odd
 *            number of decimal digits, exactly one zero at the middle, no
 *            other zeros)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 1144, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   is_cyclops_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_cyclops_sq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next = 0;                 /* start at 0 to include a(1)=0 */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t m = (seq_t)next;
        seq_t sq = m * m;
        if (is_cyclops_sq(sq)) {
            if (cache_len >= cache_cap) {
                idx_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);           /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = m;
        }
        ++next;
    }
    return cache[n-1];
}
