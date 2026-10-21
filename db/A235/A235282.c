/*
 * A235282  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 6*a(n-3) for n>=4, with a(1)=20,
 *            a(2)=40, a(3)=68
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;
static idx_t a_cache_cap = 0;

static void a_grow(idx_t needed)
{
    if (needed <= a_cache_cap) return;
    idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);          /* memory allocation failed */
    a_cache    = tmp;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for n < 1 */
    if (a_cache_len == 0) {
        a_grow(3);
        a_cache[0] = (seq_t)20;
        a_cache[1] = (seq_t)40;
        a_cache[2] = (seq_t)68;
        a_cache_len = 3;
    }
    if (n <= a_cache_len) return a_cache[n-1];

    a_grow(n);
    for (idx_t i = a_cache_len + 1; i <= n; ++i) {
        a_cache[i-1] = a_cache[i-2] * 2
                     + a_cache[i-3] * 3
                     - a_cache[i-4] * 6;
    }
    a_cache_len = n;
    return a_cache[n-1];
}
