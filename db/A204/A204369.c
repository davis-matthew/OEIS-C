/*
 * A204369  Number of (n+2) X 9 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A204369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6)  for n > 9
 * Initial values (OEIS offset 1):
 *   a(1) = 13693, a(2) = 11671, a(3) = 6786, a(4) = 6772,
 *   a(5) = 7376,  a(6) = 9272,  a(7) = 12896, a(8) = 24064,
 *   a(9) = 54880
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of allocated elements (indices 0..len-1) */

    /* Grow cache if needed */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* initialise new entries */
        for (idx_t i = old_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 0;  /* unused */
            } else if (i == 1) {
                cache[i] = 13693;
            } else if (i == 2) {
                cache[i] = 11671;
            } else if (i == 3) {
                cache[i] = 6786;
            } else if (i == 4) {
                cache[i] = 6772;
            } else if (i == 5) {
                cache[i] = 7376;
            } else if (i == 6) {
                cache[i] = 9272;
            } else if (i == 7) {
                cache[i] = 12896;
            } else if (i == 8) {
                cache[i] = 24064;
            } else if (i == 9) {
                cache[i] = 54880;
            } else {
                /* recurrence: a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) */
                seq_t t1 = 3 * cache[i - 1];
                seq_t t2 = 4 * cache[i - 6];
                seq_t sum = t1 + t2;
                seq_t t3 = 6 * cache[i - 4];
                cache[i] = sum - t3;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
