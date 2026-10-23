/*
 * A035012  One half of 9-factorial numbers.
 * https://oeis.org/A035012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035012: One half of 9‑factorial numbers
 *
 * Recurrence:
 *   a(1) = 1
 *   a(n) = (9*n-7) * a(n-1)   (n ≥ 2)
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset is 1 */

    /* cache holds a(1) .. a(current_length) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements stored */

    /* initialise cache */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed */
        cache[0] = 1;                /* a(1) = 1 */
        cache_len = 1;
    }

    /* extend cache if required */
    if ((idx_t)cache_len < n) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failed, keep old cache */
        cache = tmp;

        /* compute missing values */
        for (idx_t k = (idx_t)cache_len; k < n; ++k) {
            idx_t term = k + 1;  /* actual n for this step */
            seq_t mult = ((seq_t)9 * (seq_t)term) - 7;
            cache[k] = cache[k - 1] * mult;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
