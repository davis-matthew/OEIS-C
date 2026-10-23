/*
 * A032844  Numbers whose base-6 representation Sum_{i=0..m} d(i)*6^i has d(m)
 *          < d(m-1) > d(m-2) < ...
 * https://oeis.org/A032844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   valid_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032844: Numbers whose base-6 digits alternate up/down
 * starting with "<".  Computed by searching sequentially until the
 * required index is reached.  A static cache of already generated
 * terms ensures efficient random‑order access. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/valid_base6.h"

/* sequence a(n) */
seq_t a(idx_t n) {
    /* static cache holds the sequence values already generated */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of terms stored  */
    static idx_t cache_cap   = 0;      /* allocated capacity      */
    static seq_t next_candidate = (seq_t)1;   /* number to test next     */

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        if (valid_base6(next_candidate)) {
            if (cache_size == cache_cap) {
                idx_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - (idx_t)1];
}
