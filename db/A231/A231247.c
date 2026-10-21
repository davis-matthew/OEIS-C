/*
 * A231247  Number of 2 X n 0..3 arrays x(i,j) with each element horizontally,
 *          diagonally or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 4, no adjacent elements equal, and upper left
 *          element zero.
 * https://oeis.org/A231247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 8*a(n-3) + 4*a(n-4) for n>5 with
 *            a(1)=0, a(2)=2, a(3)=14, a(4)=44, a(5)=146
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static size_t cache_cap = 0;

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of sequence range */

    if (cache_size == 0) {                /* initialise base values */
        ensure_capacity(5);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)44;
        cache[4] = (seq_t)146;
        cache_size = 5;
    }

    if (n <= cache_size)                 /* already computed */
        return cache[n - 1];

    ensure_capacity((size_t)n);           /* grow cache if needed */
    for (idx_t i = cache_size; i < n; ++i) {
        /* recurrence: a(i+1) = 4*a(i) - 3*a(i-1) + 8*a(i-2) + 4*a(i-3) */
        cache[i] = 4 * cache[i - 1] - 3 * cache[i - 2]
                 + 8 * cache[i - 3] + 4 * cache[i - 4];
    }
    cache_size = n;
    return cache[n - 1];
}
