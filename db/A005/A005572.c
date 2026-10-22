/*
 * A005572  Number of walks on cubic lattice starting and finishing on the xy
 *          plane and never going below it.
 * https://oeis.org/A005572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy, nice
 * Formula:   a(0) = 1, a(1) = 4; for n > 1 (arbitrary precision) a(n) =
 *            ((2*n+1)*a(n-1) - 3*(n-1)*a(n-2))*4/(n+2); for fixed-width seq_t
 *            a(n) = 4*a(n-1) + Sum_{i=1..n-1} a(i-1)*a(n-i-1), whose terms
 *            are all <= a(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *cache = NULL;
static idx_t have = 0;
static idx_t cap = 0;

seq_t a(idx_t n) {
    if (n < have) return cache[n];
    if (n >= cap) {
        idx_t newcap = (n + 1) * 2;
        if (newcap < 64) newcap = 64;
        if (cache == NULL) cache = alloc_zero(newcap);
        else cache = grow_zero(cache, cap, newcap);
        cap = newcap;
    }
    if (have == 0) {
        cache[0] = 1;
        cache[1] = 4;
        have = 2;
    }
    int big = seq_is_big();
    for (idx_t m = have; m <= n; m++) {
        if (big) {
            seq_t ms = (seq_t)m;
            seq_t num = (2 * ms + 1) * cache[m - 1] - 3 * (ms - 1) * cache[m - 2];
            cache[m] = num * 4 / (ms + 2);
        } else {
            seq_t s = 4 * cache[m - 1];
            for (idx_t i = 1; i < m; i++) {
                s += cache[i - 1] * cache[m - 1 - i];
            }
            cache[m] = s;
        }
    }
    have = n + 1;
    return cache[n];
}
