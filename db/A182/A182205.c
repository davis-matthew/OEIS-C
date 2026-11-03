/*
 * A182205  Iterate the map in A006368 starting at 40.
 * https://oeis.org/A182205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = A006368(a(n)), a(0) = 40 A006368 is defined by if n is
 *            even: f(n) = 3n / 2 if n is odd: f(n) = round(3n / 4) (i.e. (3n
 *            + 2)/4)
 * Range:     u8 n <= 9, u16 n <= 71, u32 n <= 339, u64 n <= 720,
 *            u128 n <= 1683, big any n
 * Validated: standard, pass, 65 terms through n = 9982 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static inline seq_t next_step(seq_t x)
{
    if ((x & 1) == 0)           /* even: 3x/2 = (x/2)*3  */
        return (x / 2) * 3;
    else                        /* odd:  (3x+2)/4  */
    {
        seq_t q = x / 4;        /* floor(x/4)           */
        seq_t r = x & 3;        /* x mod 4 (0..3)        */
        /* (r*3 + 2)/4 is 0 or 1, never overflow */
        return q * 3 + (r * 3 + 2) / 4;
    }
}

seq_t a(idx_t n)
{
    /* static cache to allow any calling order */
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;       /* current allocated size */
    static idx_t  cache_len = 0;       /* highest index stored */

    if (cache == NULL) {                /* first call: allocate one slot */
        cache_cap = 1;
        cache = mem_new(seq_t, 1);
        if (!cache) return (seq_t)0;    /* allocation failure – unlikely in tests */
        cache[0] = 40;                  /* a(0) */
        cache_len = 0;
    }

    if (n <= cache_len)                 /* already known */
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return (seq_t)0;     /* allocation failure – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    seq_t curr = cache[cache_len];
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        curr = next_step(curr);
        cache[i] = curr;
    }
    cache_len = n;
    return curr;
}
