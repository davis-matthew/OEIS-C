/*
 * A233020  Number of n X 2 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, vertically, diagonally or
 *          antidiagonally, and top left element zero.
 * https://oeis.org/A233020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2); a(1)=3, a(2)=15
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;         /* a(1), a(2), … */
    static idx_t cache_len = 0;         /* number of terms computed (max index+1) */
    static idx_t cache_cap = 0;         /* array capacity in terms */

    /* Ensure capacity for at least 'n' terms */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        assert(tmp && "realloc failed");
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize the first two terms (only once) */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;      /* a(1) */
        cache[1] = (seq_t)15;     /* a(2) */
        cache_len = 2;            /* we have computed two terms */
    }

    /* Compute missing terms up to n */
    if (n > cache_len) {
        for (idx_t i = cache_len; i < n; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)2 * cache[i - 2];
        }
        cache_len = n;
    }

    return cache[n - 1];           /* a(n) is stored at index n-1 */
}
