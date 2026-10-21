/*
 * A230904  Number of n X 2 0..3 arrays x(i,j) with each element horizontally,
 *          vertically or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A230904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0, a(2)=2, a(3)=8, a(4)=26,
   a(n)=5*a(n-1)-6*a(n-2)+a(n-3) for n>4 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* safety, not used for n≥1 */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;               /* array size (indices 0..len-1) */
    static idx_t max_cached = 0;              /* maximum index with defined value */

    if (cache == NULL) {
        cache_len = (n >= 4) ? n + 1 : 5;      /* at least indices 0..4 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 8;
        cache[4] = 26;
        max_cached = 4;
    }

    if (n > max_cached) {
        if (n >= cache_len) {
            idx_t new_len = n + 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
            cache_len = new_len;
        }
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1] - (seq_t)6 * cache[i - 2] + cache[i - 3];
        }
        max_cached = n;
    }

    return cache[n];
}
