/*
 * A107850  Expansion of g.f.
 *          (x^2+x+1)*(2*x^2+2*x+1)*(x-1)^2/((1-x^2-2*x^3)*(x^4+1)).
 * https://oeis.org/A107850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) - a(n-4) + a(n-6) + 2*a(n-7) (initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=0, a(4)=1, a(5)=1, a(6)=3)
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 56, u64 n <= 108,
 *            u128 n <= 214, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation store */
    static seq_t *cache = NULL;
    static idx_t  cached_n = (idx_t)-1;   /* highest index computed so far */
    static size_t cap = 0;                /* current capacity of cache */

    /* the first seven terms */
    static const seq_t init[7] = {1, 1, 1, 0, 1, 1, 3};

    /* first call: create cache and fill initial terms */
    if (cache == NULL) {
        size_t required = (size_t)n + 1;
        /* choose a capacity large enough for the initial terms */
        cap = 8;
        if (required > cap) {
            /* for small n we can just use the required size */
            if (required <= 16) {
                cap = required;
            } else {
                /* otherwise use a power‑of‑two size for efficiency */
                while (cap < required) {
                    cap <<= 1;
                }
            }
        }

        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) {
            return 0;  /* allocation failure – small fallback */
        }

        /* fill the known terms */
        for (idx_t i = 0; i < 7; ++i) {
            cache[i] = init[i];
        }
        cached_n = 6;   /* we have computed up to a(6) */
    }

    /* quick answer for the first seven terms */
    if (n < 7) {
        return init[n];
    }

    /* enlarge cache if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap;
        while (needed > newcap) {
            newcap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) {
            return 0;  /* allocation failure */
        }
        cache = tmp;
        cap = newcap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* recurrence valid for i ≥ 7 because we already have cache[0..6] */
        seq_t value = cache[i - 2]
                    + (seq_t)2 * cache[i - 3]
                    - cache[i - 4]
                    + cache[i - 6]
                    + (seq_t)2 * cache[i - 7];
        cache[i] = value;
    }
    cached_n = n;

    return cache[n];
}
