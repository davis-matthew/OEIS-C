/*
 * A200556  Number of -n..n arrays x(0..5) of 6 elements with zero sum and
 *          nonzero second differences.
 * https://oeis.org/A200556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)-2*a(n-4)-a(n-6)-a(n-7)-a(n-8) *
 *            +3*a(n-9)+3*a(n-10)-a(n-11)-a(n-12)-a(n-13)-2*a(n-15) *
 *            +a(n-16)+a(n-17)+a(n-18)-a(n-19) * Initial values: a(1)=72,
 *            a(2)=1240, a(3)=7254, a(4)=26850, a(5)=75792, * a(6)=178834,
 *            a(7)=372150, a(8)=705270, a(9)=1242888, * a(10)=2067650, a(1...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 47, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[20] = {
    (seq_t)0,   /* index 0 unused */
    (seq_t)72,  (seq_t)1240, (seq_t)7254, (seq_t)26850, (seq_t)75792,
    (seq_t)178834, (seq_t)372150, (seq_t)705270, (seq_t)1242888,
    (seq_t)2067650, (seq_t)3281874, (seq_t)5009232, (seq_t)7397868,
    (seq_t)10621792, (seq_t)14882672, (seq_t)20413220, (seq_t)27478160,
    (seq_t)36376230, (seq_t)47443518
};

seq_t a(idx_t n)
{
    /* static cache for memoisation (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first 19 terms */
    if (cache == NULL) {
        cache_size = 19;
        /* use explicit cast for C++ compatibility */
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;               /* allocation failed */

        for (idx_t i = 0; i <= cache_size; ++i) {
            cache[i] = init_vals[i];
        }
    }

    /* compute any missing terms up to n */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (new_size + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;           /* allocation failed */
        cache = new_cache;

        /* recurrence for i = cache_size+1 .. n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] =
                cache[i-1] + cache[i-2] + cache[i-3]
                - 2*cache[i-4] - cache[i-6] - cache[i-7] - cache[i-8]
                + 3*cache[i-9] + 3*cache[i-10]
                - cache[i-11] - cache[i-12] - cache[i-13]
                - 2*cache[i-15] + cache[i-16] + cache[i-17] + cache[i-18]
                - cache[i-19];
        }
        cache_size = n;
    }

    return cache[n];
}
