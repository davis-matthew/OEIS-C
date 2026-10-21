/*
 * A221964  Number of -5..5 arrays of length n with the sum ahead of each
 *          element differing from the sum following that element by 5 or
 *          less.
 * https://oeis.org/A221964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 18*a(n-2) - 20*a(n-3) - 48*a(n-4) + 16*a(n-5)
 *            + 32*a(n-6) initial values: a(1)=11, a(2)=121, a(3)=671,
 *            a(4)=4961, a(5)=34111, a(6)=241153
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;      /* Array of computed terms, index 0 unused */
    static idx_t cache_len = 0;      /* Number of elements currently stored */

    if (n < 1) return (seq_t)0;          /* Out of range */

    if (cache_len <= n) {
        size_t needed_len = (size_t)n + 1;          /* Elements 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, needed_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                 /* Allocation failure */
        cache = tmp;

        idx_t start = cache_len;
        if (start < 1) start = 1;
        for (idx_t i = start; i <= n; ++i) {
            if (i == 1) cache[i] = (seq_t)11;
            else if (i == 2) cache[i] = (seq_t)121;
            else if (i == 3) cache[i] = (seq_t)671;
            else if (i == 4) cache[i] = (seq_t)4961;
            else if (i == 5) cache[i] = (seq_t)34111;
            else if (i == 6) cache[i] = (seq_t)241153;
            else {
                seq_t p = 5 * cache[i-1] + 18 * cache[i-2]
                        + 16 * cache[i-5] + 32 * cache[i-6];
                seq_t ntr = 20 * cache[i-3] + 48 * cache[i-4];
                cache[i] = p - ntr;
            }
        }
        cache_len = needed_len;
    }

    return cache[n];
}
