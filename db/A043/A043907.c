/*
 * A043907  Numbers k such that 2 and 5 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_adjacent_25_52
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth integer k such that the base‑6 representation of k contains the
 *         digits 2 and 5 next to each other (either “25” or “52”) and the
 *         base‑6 representation of k+1 does NOT contain such digits.
 * This implementation searches the integers sequentially and caches the
 * results. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_25_52.h"

seq_t a(idx_t n)
{
    /* static state: cached sequence and next integer to test */
    static seq_t *seq = NULL;
    static size_t len = 0;
    static size_t cap = 0;
    static seq_t cur = 0;   /* next integer to be examined */

    if (n == 0)
        return 0;   /* sequence is defined for n ≥ 1 */

    /* Generate numbers until we have at least n entries in the cache */
    while (len < n) {
        if (has_adjacent_25_52(cur) && !has_adjacent_25_52(cur + 1)) {
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!seq)
                    abort();   /* allocation failure */
                cap = newcap;
            }
            seq[len++] = cur;
        }
        cur += 1;
    }
    return seq[n - 1];
}
