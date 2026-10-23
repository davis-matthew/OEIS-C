/*
 * A045784  Squares with initial digit '1'.
 * https://oeis.org/A045784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A045855(n)^2 Sequence A045784: squares whose decimal
 *            representation starts with the digit 1. The function generates
 *            the terms by scanning positive integers, squaring them, and
 *            checking the most significant decimal digit. Results are
 *            memoized so subsequent calls are fast and independent of...
 * Range:     u8 n <= 7, u16 n <= 62, u32 n <= 9966, u64 n <= 9966,
 *            u128 n <= 9966, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* undefined for n < 1 – return 0 */

    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;
    static idx_t  last_x    = 0;        /* largest integer already examined */

    /* Expand cache capacity if necessary. */
    if (cache_len < n) {
        if (cache_cap < n) {
            idx_t new_cap = n * 2 + 8;  /* extra room for future growth */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate new terms until we have at least n elements. */
        seq_t ten = (seq_t)10;
        while (cache_len < n) {
            ++last_x;
            seq_t sq = (seq_t)last_x * last_x;    /* square of the integer */
            seq_t msd = sq;
            while (msd >= ten)                    /* find most significant digit */
                msd /= ten;
            if (msd == (seq_t)1)                  /* leading digit is 1 */
                cache[cache_len++] = sq;
        }
    }

    return cache[n - 1];                 /* 1‑based indexing matches OEIS */
}
