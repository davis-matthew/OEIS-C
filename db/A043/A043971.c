/*
 * A043971  Numbers k such that 1 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has_adjacent_1_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th integer k whose base‑9 representation
   contains two adjacent digits 1 and 5 (in any order) while
   k+1’s base‑9 representation does not.   */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_1_5.h"

seq_t a(idx_t n)
{
    /* Return the n‑th term of the sequence (n≥1).  
       Uses memoisation so repeated calls are efficient.  */
    static seq_t *cache = NULL;
    static size_t sz = 0;      /* number of cached terms */
    static size_t cap = 0;     /* allocated capacity of cache */
    static idx_t curk = 0;     /* last checked k */

    if (n == 0) return 0; /* behaviour for n<1 is irrelevant */

    /* initialise cache */
    if (cache == NULL) {
        cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
    }

    /* extend cache until it contains at least n terms */
    while (sz < n) {
        curk++;                     /* next candidate k */
        seq_t k = curk;

        if (has_adjacent_1_5(k)) {
            seq_t kp1 = k + 1;
            if (!has_adjacent_1_5(kp1)) {
                if (sz == cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
                }
                cache[sz++] = k;
            }
        }
    }

    return cache[n - 1];
}
