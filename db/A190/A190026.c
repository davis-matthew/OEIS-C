/*
 * A190026  Number of (n+2) X 4 binary arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A190026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 24, u32 n <= 60, u64 n <= 132, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A190026: Number of (n+2)×4 binary arrays such that every 3×3
   subblock commutes with its horizontal and vertical neighbors.
   Recurrence (valid for n > 10):
       a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5) + a(n-6) - a(n-7)
   Initial values:
       a(1)=112, a(2)=45,  a(3)=79,  a(4)=155,  a(5)=235,
       a(6)=295, a(7)=355,  a(8)=423,  a(9)=531, a(10)=740
   The function caches computed values to answer any sequence index in
   O(n) time (overall up to the maximum n requested) and consumes
   only O(n) space.  It is safe for any integer type seq_t.
*/

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: index 0 unused, cache[i] = a(i) for 1 <= i <= max_known */
    static seq_t *cache = NULL;
    static idx_t max_known = 0;

    /* initialise cache with the 10 known terms */
    if (cache == NULL) {
        cache = (seq_t*)malloc(11 * sizeof(seq_t));  /* indices 0..10 */
        if (!cache) return 0;                     /* allocation failed */
        cache[0] = 0;                          /* unused */
        cache[1] = (seq_t)112;
        cache[2] = (seq_t)45;
        cache[3] = (seq_t)79;
        cache[4] = (seq_t)155;
        cache[5] = (seq_t)235;
        cache[6] = (seq_t)295;
        cache[7] = (seq_t)355;
        cache[8] = (seq_t)423;
        cache[9] = (seq_t)531;
        cache[10] = (seq_t)740;
        max_known = 10;
    }

    if (n <= max_known) return cache[n];

    /* extend the cache up to n */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return 0;            /* allocation failed */
    cache = new_cache;

    for (idx_t i = max_known + 1; i <= n; ++i) {
        /* sum = 2*a(i-1) + a(i-3) + a(i-5) + a(i-6) */
        /* sub = a(i-2) + 2*a(i-4) + a(i-7)  */
        seq_t sum = 2 * cache[i-1] + cache[i-3] + cache[i-5] + cache[i-6];
        seq_t sub = cache[i-2] + 2 * cache[i-4] + cache[i-7];
        cache[i] = sum - sub;            /* guaranteed non‑negative */
    }
    max_known = n;
    return cache[n];
}
