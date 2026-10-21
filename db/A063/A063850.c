/*
 * A063850  Say what you see in previous term, reporting total number for each
 *          digit encountered.
 * https://oeis.org/A063850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(0) = 1; for n > 0, a(n) is obtained by reading the previous
 *            term a(n‑1), counting how many times each digit 0‑9 appears
 *            (processing the distinct digits in order of first appearance),
 *            and writing each count followed by the digit
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   next_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/next_term.h"

/* memoised evaluation of a(n) */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    if (n < cached_len) return cache[n];

    /* extend cache to size n+1 */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!cache) {
        /* allocation failure – abort */
        return 0;
    }

    /* initialise new values */
    for (idx_t i = cached_len; i < new_len; i++) {
        if (i == 0) {
            cache[0] = 1;
        } else {
            cache[i] = next_term(cache[i - 1]);
        }
    }
    cached_len = new_len;
    return cache[n];
}
