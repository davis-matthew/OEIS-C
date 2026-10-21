/*
 * A229660  10-section a(10n+k) gives k-th differences of a for k=0..9 with
 *          a(n)=0 for n<9 and a(9)=1.
 * https://oeis.org/A229660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look, eigen
 * Formula:   a(10*n + k) = Σ_{j=0..k} (-1)^j * C(k,j) * a(n + k - j) (k=0..9)
 *            Initial values: a(m) = 0 for m < 9; a(9) = 1
 * Range:     i8 n <= 128, i16 n <= 1238, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int BINOM[10][10] = {
    {1},
    {1, 1},
    {1, 2, 1},
    {1, 3, 3, 1},
    {1, 4, 6, 4, 1},
    {1, 5,10,10, 5, 1},
    {1, 6,15,20,15, 6, 1},
    {1, 7,21,35,35,21, 7, 1},
    {1, 8,28,56,70,56,28, 8, 1},
    {1, 9,36,84,126,126,84,36, 9, 1}
};

/* Compute a(n) for any n ≥ 0, caching previously computed values. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of values stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (needed > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 256;
            while (new_cap < needed) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        /* compute missing values */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i < 9) {                     /* a(m) = 0 for m < 9 */
                cache[i] = (seq_t)0;
            } else if (i == 9) {              /* a(9) = 1 */
                cache[i] = (seq_t)1;
            } else {
                idx_t block = i / 10;         /* n in the recurrence   */
                int k = (int)(i % 10);        /* k in the recurrence   */
                seq_t sum = 0;
                for (int j = 0; j <= k; ++j) {
                    seq_t term = cache[block + k - j];
                    if (j & 1) {                 /* (-1)^j = -1 when j odd */
                        sum -= (seq_t)BINOM[k][j] * term;
                    } else {
                        sum += (seq_t)BINOM[k][j] * term;
                    }
                }
                cache[i] = sum;
            }
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
