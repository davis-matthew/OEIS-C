/*
 * A043144  Numbers k such that 2 and 5 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adjacent_2_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search based implementation: iterate successive integers k and
 * test whether 2 and 5 occur as adjacent digits in base‑7 of k but
 * not of k‑1.  The sequence is memoised in a static cache so
 * repeated calls with arbitrary n remain fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_2_5.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for offset 0 */

    /* static cache for already discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static state to continue scanning from where we left off */
    static seq_t cur = 1;          /* next candidate to test */
    static bool prev_has = false;  /* has_adjacent_2_5(cur-1) */

    /* grow the cache until it contains the n‑th term */
    while (cache_len < n) {
        bool cur_has = has_adjacent_2_5(cur);

        if (cur_has && !prev_has) {
            /* store the term */
            if (cache_len == cache_cap) {
                size_t newcap = cache_cap ? cache_cap * 2U : 16U;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* out of memory */
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = cur;
        }

        prev_has = cur_has;
        ++cur;                       /* next integer to examine */
    }

    return cache[n - 1];             /* offset 1 */
}
