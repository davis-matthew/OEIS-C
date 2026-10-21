/*
 * A224160  Number of 4 X n 0..1 arrays with diagonals and rows unimodal and
 *          antidiagonals nondecreasing.
 * https://oeis.org/A224160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) Initial
 *            values: a(1)=16, a(2)=108, a(3)=281, a(4)=574, a(5)=1156,
 *            a(6)=2271, a(7)=4339, a(8)=8008, a(9)=14257, a(10)=24519,
 *            a(11)=40840, a(12)=66082, a(13)=104179
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 59, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Edge: the OEIS offset is 1, so n>=1 in normal use */
    if (n == 0) return 0;

    static const seq_t init[13] = {
        (seq_t)16,  (seq_t)108, (seq_t)281,  (seq_t)574,
        (seq_t)1156, (seq_t)2271, (seq_t)4339, (seq_t)8008,
        (seq_t)14257,(seq_t)24519,(seq_t)40840, (seq_t)66082,
        (seq_t)104179
    };

    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently cached (n value) */
    static size_t cache_cap = 0;   /* allocated capacity in terms */

    /* Expand cache if necessary */
    if ((size_t)n > cache_len) {
        size_t new_cap = cache_cap ? cache_cap : n;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* out of memory; caller may not handle this */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            if (i < 13) {
                cache[i] = init[i];
            } else {
                /* recurrence: a(i+1) depends on a(i)...a(i-8) */
                seq_t val =
                        (seq_t)9 *   cache[i-1]
                      - (seq_t)36 *  cache[i-2]
                      + (seq_t)84 *  cache[i-3]
                      - (seq_t)126 * cache[i-4]
                      + (seq_t)126 * cache[i-5]
                      - (seq_t)84  * cache[i-6]
                      + (seq_t)36  * cache[i-7]
                      - (seq_t)9   * cache[i-8]
                      +            cache[i-9];
                cache[i] = val;
            }
        }
        cache_len = (size_t)n;
    }
    return cache[(size_t)n - 1];
}
