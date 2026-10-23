/*
 * A115108  Expansion of -(9520*x^9 -11504*x^8 +2840*x^7 -1040*x^6 +248*x^5
 *          +36*x^4 -1) / ((2*x -1)*(4*x^2 +2*x -1)).
 * https://oeis.org/A115108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*a(n-1) - 8*a(n-3) + b(n), with b(0)=1, b(4)=-36,
 *            b(5)=-248, b(6)=1040, b(7)=-2840, b(8)=11504, b(9)=-9520, and
 *            b(k)=0 for k>9. Initial values: a(0)=1, a(1)=4, a(2)=16
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static const int32_t B[10] = {
    1,   0,   0,   0, -36, -248, 1040, -2840, 11504, -9520
};

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid entries = max index + 1 */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Ensure the cache can hold up to index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (n + 1) * 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) return 0;   /* allocation failure: return 0 */
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)4;
        } else if (i == 2) {
            cache[2] = (seq_t)16;
        } else {
            /* Use 128‑bit intermediate to avoid overflow on 64‑bit builds */
            unsigned __int128 tmp =
                (unsigned __int128)cache[i - 1] * 4U -
                (unsigned __int128)cache[i - 3] * 8U;

            if (i <= 9) {   /* add the inhomogeneous term b(i) */
                if (B[i] >= 0)
                    tmp += (unsigned __int128)B[i];
                else
                    tmp -= (unsigned __int128)(-B[i]);
            }
            cache[i] = (seq_t)tmp;
        }
    }
    cache_len = (size_t)(n + 1);
    return cache[n];
}
