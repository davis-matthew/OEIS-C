/*
 * A235717  Squares which have one or more occurrences of exactly two
 *          different digits.
 * https://oeis.org/A235717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the nth square whose decimal representation has exactly two distinct digits. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_candidate = 1; /* candidate for the next square root */

    /* generate terms until we have enough */
    while (cache_size < n) {
        seq_t cand = (seq_t)next_candidate;
        seq_t sq   = cand * cand;

        unsigned char cnt[10];
        digit_counts(sq, cnt);

        int distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (cnt[d]) ++distinct;

        if (distinct == 2) {
            /* ensure enough storage */
            if (cache_cap == cache_size) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – abort with null */
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = sq;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
