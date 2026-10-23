/*
 * A169533  Number of reduced words of length n in Coxeter group on 40
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A169533: Number of reduced words of length n in a Coxeter
 * group on 40 generators with relations (S_i)^2 = I and (S_i S_j)^34 = I
 * Recurrence:
 *   a(0) = 1
 *   a(1) = 40
 *   a(n) = 39 * a(n-1)          for 2 ≤ n ≤ 33
 *   a(n) = 38 * (a(n-1)+…+a(n-33)) - 741 * a(n-34)   for n ≥ 34
 */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;   /* cached terms a(0)…a(k) */
    static size_t cache_len = 0;      /* number of terms cached */

    if ((size_t)n + 1 > cache_len) {
        /* enlarge cache array */
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – return 0 as a harmless fallback */
            return (seq_t)0;
        }
        cache = tmp;

        /* constant values as seq_t */
        const seq_t c1   = (seq_t)1;
        const seq_t c40  = (seq_t)40;
        const seq_t c39  = (seq_t)39;
        const seq_t c38  = (seq_t)38;
        const seq_t c741 = (seq_t)741;

        /* compute new terms */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[0] = c1;
            } else if (i == 1) {
                cache[1] = c40;
            } else if (i <= 33) {
                cache[i] = c39 * cache[i - 1];
            } else { /* i >= 34 */
                seq_t sum = (seq_t)0;
                for (size_t j = 1; j <= 33; ++j) {
                    sum += cache[i - j];
                }
                cache[i] = c38 * sum - c741 * cache[i - 34];
            }
        }

        cache_len = new_len;
    }

    return cache[(size_t)n];
}
