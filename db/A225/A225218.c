/*
 * A225218  Square numbers containing all the digits 0, 1, 2, 3, 4, 5, 6, 7, 8
 *          and 9.
 * https://oeis.org/A225218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 4866, u128 n <= 4866,
 *            big any n
 * Helpers:   digit_counts, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/digit_counts.h"

/* This function returns the n‑th square number that contains all ten decimal
   digits (0–9). The sequence is defined by searching for the n‑th such square. */
seq_t a(idx_t n)
{
    /* static memoisation data */
    static seq_t *cache = NULL;      /* array of found squares          */
    static size_t cache_len = 0;     /* number of squares stored       */
    static size_t cache_cap = 0;     /* current capacity of cache      */
    static idx_t next_root = 1;      /* next integer to test as a root */

    if (n == 0)            /* offset is 1; undefined for n = 0 */
        return (seq_t)0;

    /* if we have already computed this term, return it */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* generate new terms until we reach the desired index */
    while (cache_len < n) {
        /* compute the square of the current root */
        seq_t root_val = (seq_t)next_root;
        seq_t sq = seq_mul(root_val, root_val);

        /* count digits in the square */
        unsigned char cnt[10] = {0};
        digit_counts(sq, cnt);

        /* test if all digits 0–9 appear at least once */
        bool full = true;
        for (int d = 0; d < 10; ++d) {
            if (cnt[d] == 0) {
                full = false;
                break;
            }
        }

        if (full) {
            /* grow the cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!tmp) abort();
                for (size_t i = 0; i < cache_len; ++i)
                    tmp[i] = cache[i];
                free(cache);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;   /* store found square */
        }

        ++next_root;
    }

    return cache[n - 1];
}
