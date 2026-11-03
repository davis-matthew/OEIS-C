/*
 * A134512  Row sums of triangle A134511.
 * https://oeis.org/A134512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=3, a(3)=4, a(4)=10, a(5)=14 For n ≥ 6: a(n)
 *            = a(n-1) + 4*a(n-2) + a(n-5) + a(n-6) - 3*a(n-3) - 4*a(n-4)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 41, u64 n <= 86, u128 n <= 177,
 *            big any n
 * Validated: standard, pass, 57 terms through n = 1903 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;   /* cached values */
    static size_t cap   = 0;       /* capacity of cache */
    static size_t len   = 0;       /* number of computed terms */

    /* requested index >= 0 */
    size_t need = (size_t)n + 1;   /* number of elements that must be stored */

    /* grow storage if necessary */
    if (need > cap) {
        seq_t *new_cache = seq_alloc_new_array(need);
        if (cache) {
            for (size_t i = 0; i < len; ++i)
                new_cache[i] = cache[i];
            seq_free_array(cache);
        }
        cache = new_cache;
        cap   = need;
    }

    /* make sure the first six terms are defined */
    if (len < 6) {
        const seq_t init[6] = { (seq_t)1, (seq_t)1, (seq_t)3,
                                (seq_t)4, (seq_t)10, (seq_t)14 };
        size_t to_copy = 6 < need ? 6 : need;
        for (size_t i = 0; i < to_copy; ++i)
            cache[i] = init[i];
        len = to_copy;
    }

    /* compute missing terms */
    for (size_t i = len; i < need; ++i) {
        seq_t s = cache[i-1];
        s += mul_small(cache[i-2], (unsigned)4);
        s += cache[i-5];
        s += cache[i-6];

        seq_t t = mul_small(cache[i-3], (unsigned)3);
        t += mul_small(cache[i-4], (unsigned)4);

        cache[i] = s - t;   /* s ≥ t for all n */
    }
    len = need;

    return cache[(size_t)n];
}
