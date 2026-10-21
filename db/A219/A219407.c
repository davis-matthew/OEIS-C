/*
 * A219407  Unmatched value maps: number of nX5 binary arrays indicating the
 *          locations of corresponding elements not equal to any horizontal or
 *          antidiagonal neighbor in a random 0..2 nX5 array.
 * https://oeis.org/A219407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A219407 - Unmatched value maps
   Recurrence: a(n) = 12*a(n-1) + 230*a(n-2) + 999*a(n-3) + 1175*a(n-4)
                - 1025*a(n-5) - 1129*a(n-6) + 1240*a(n-7) - 405*a(n-8) + 45*a(n-9)
   Initial values (n = 1..10):
        12, 351, 8173, 193116, 4560446, 107701719,
        2543481662, 60067211485, 1418553120783, 33500701298909
   a(1) is the first term; 0 is returned for n == 0 (no definition). */

#include <stdlib.h>

/* Base values for n = 1 .. 10 */
static const seq_t base_vals[10] = {
    12, 351, 8173, 193116, 4560446, 107701719,
    2543481662ULL, 60067211485ULL, 1418553120783ULL, 33500701298909ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – zero is undefined, but safe to return 0 */

    static seq_t *cache = NULL;    /* memoization table */
    static idx_t cache_len = 0;    /* number of items stored (max index) */

    /* If the requested term is already cached, return it quickly. */
    if (n <= cache_len) return cache[n-1];

    /* Grow the cache to size n.  realloc works on void*; cast to seq_t* for C++ compatibility. */
    cache = (seq_t *)realloc(cache, n * sizeof(seq_t));
    if (!cache) exit(1);           /* malloc failure – abort */

    /* Compute all new terms up to n.  Terms 1..10 are stored unchanged. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 10) {
            cache[i-1] = base_vals[i-1];
        } else {
            cache[i-1] =
                12     * cache[i-2] +   /* a(i-1) */
                230    * cache[i-3] +   /* a(i-2) */
                999    * cache[i-4] +   /* a(i-3) */
                1175   * cache[i-5] +   /* a(i-4) */
                (-1025) * cache[i-6] +  /* a(i-5) */
                (-1129) * cache[i-7] +  /* a(i-6) */
                1240   * cache[i-8] +   /* a(i-7) */
                (-405)  * cache[i-9] +   /* a(i-8) */
                45     * cache[i-10];    /* a(i-9) */
        }
    }

    cache_len = n;                  /* record the new cache size */
    return cache[n-1];
}
