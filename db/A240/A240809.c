/*
 * A240809  a(n) = n for 1 <= n <= 4; thereafter a(n) = a(n - a(n-2)) + a(n -
 *          a(n-4)).
 * https://oeis.org/A240809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n - a(n-2)) + a(n - a(n-4)), with a(1)=1, a(2)=2,
 *            a(3)=3, a(4)=4
 * Range:     u8 n <= 381, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 108 terms through n = 9979 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit */
#include "../utils/mem.h"

static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* Grow cache to hold indices 0..n (index 0 unused) */
    idx_t needed = n + 1;
    if (cache_cap < needed) {
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)needed);
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = needed;
    }

    /* Initial values */
    if (n >= 1) cache[1] = (seq_t)1;
    if (n >= 2) cache[2] = (seq_t)2;
    if (n >= 3) cache[3] = (seq_t)3;
    if (n >= 4) cache[4] = (seq_t)4;

    /* Recurrence for n≥5 */
    for (idx_t i = 5; i <= n; ++i) {
        idx_t j1 = i - (idx_t)cache[i - 2];
        idx_t j4 = i - (idx_t)cache[i - 4];
        cache[i] = cache[j1] + cache[j4];
    }

    return cache[n];
}
