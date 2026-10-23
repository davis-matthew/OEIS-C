/*
 * A188559  Number of 8 X n binary arrays without the pattern 0 1 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 62, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) + 126*a(n-5)
   - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9)   for n>9,
   with a(1)=9, a(2)=17, a(3)=32, a(4)=60, a(5)=112,
   a(6)=208, a(7)=384, a(8)=704, a(9)=1280.   */
#include <stdlib.h>
#include <stdint.h>

static inline void ensure_cache(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap > 0) ? (*cap << 1) : 1;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    *arr = tmp;
    *cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n<1 */
    static const seq_t init_vals[10] = {
        0,        /* dummy for index 0 */
        9, 17, 32, 60, 112,
        208, 384, 704, 1280
    };
    if (n <= 9) return init_vals[(idx_t)n];

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;

    if (cache_max == 0) {
        /* initialise cache with known values */
        ensure_cache(10, &cache, &cache_cap);   /* indices 0..9 */
        for (size_t i = 1; i <= 9; ++i) cache[i] = init_vals[i];
        cache_max = 9;
    }

    if (n > cache_max) {
        ensure_cache((size_t)(n + 1), &cache, &cache_cap);
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            seq_t val = 0;
            val += (seq_t)9  * cache[i-1];
            val -= (seq_t)36 * cache[i-2];
            val += (seq_t)84 * cache[i-3];
            val -= (seq_t)126 * cache[i-4];
            val += (seq_t)126 * cache[i-5];
            val -= (seq_t)84 * cache[i-6];
            val += (seq_t)36 * cache[i-7];
            val -= (seq_t)9  * cache[i-8];
            val += cache[i-9];
            cache[i] = val;
        }
        cache_max = n;
    }
    return cache[n];
}
