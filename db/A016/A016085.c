/*
 * A016085  a(1) = 1; a(n+1) = floor((sum{k=1 to n} a(k)^3)^(1/3)).
 * https://oeis.org/A016085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; a(n+1)=floor((∑_{k=1}^n a(k)^3)^(1/3))
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 38, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* Persistent cache */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;      /* allocated capacity */
    static idx_t  size    = 0;      /* terms stored so far */
    static seq_t sum_cubes = 0;     /* ∑ a(k)^3 for k = 1..size */

    /* The sequence is defined for n ≥ 1 */
    if (n == 0)
        return 0;

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (!cache)
            return 0;              /* allocation failed */

        /* a(1) = 1 */
        cache[0] = 1;
        sum_cubes = cache[0] * cache[0] * cache[0];
        size = 1;
    }

    /* Return cached value if already known */
    if (n <= size)
        return cache[n - 1];

    /* Generate terms up to the requested index */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t next = floor_cube_root(sum_cubes);  /* a(i) */

        /* Grow cache if needed */
        if (size >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                return 0;          /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }

        cache[size] = next;
        sum_cubes += next * next * next;
        ++size;
    }

    return cache[n - 1];
}
