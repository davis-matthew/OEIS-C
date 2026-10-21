/*
 * A200216  Danilov's sequence of x satisfying 0 < |x^3-y^2| < sqrt(x) with
 *          integer (x,y).
 * https://oeis.org/A200216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) follows the 5‑term linear recurrence
 *   a(n) = 3461450947505*a(n-1) + 6440022564931157490*a(n-2)
 *          - 6440022564931157490*a(n-3) - 3461450947505*a(n-4) + a(n-5)
 *   with initial terms
 *     a(1) = 93844
 *     a(2) = 322001299796379844
 *     a(3) = 1114592308630995805123571151844
 *     a(4) = 3858108676488182444301031186675778188809844
 *     a(5) = 13354661111806898918013326915229994453818137920195953844
 *
 * The function caches computed values so that any n ≥ 1 can be requested
 * in any order.  The recurrence requires only additions, subtractions, and
 * multiplications by the fixed 64‑bit constants 3461450947505 and
 * 6440022564931157490, which are safe to represent in seq_t.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for zero offset – return 0 as a safe fallback. */
        return (seq_t)0;
    }

    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static bool parsed = false;

    /* Parse the first five terms once. */
    if (!parsed) {
        /* Ensure space for indices 0..5 (index 0 unused). */
        cache = (seq_t *)malloc(6 * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = 0;                  /* unused slot */

        static const char *init_strs[6] = {
            NULL,
            "93844",
            "322001299796379844",
            "1114592308630995805123571151844",
            "3858108676488182444301031186675778188809844",
            "13354661111806898918013326915229994453818137920195953844"
        };

        for (idx_t i = 1; i <= 5; ++i) {
            cache[i] = seq_from_str(init_strs[i]);
        }

        cache_len = 5;
        parsed = true;
    }

    /* Extend cache if necessary. */
    if (n > cache_len) {
        size_t newsize = (size_t)n + 1;          /* indices 0..n */
        seq_t *newcache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        if (!newcache) return (seq_t)0;          /* allocation failure */
        cache = newcache;

        /* Constants used in the recurrence.  Cast to seq_t for safety. */
        const seq_t c1 = (seq_t)3461450947505ULL;
        const seq_t c2 = (seq_t)6440022564931157490ULL;

        /* Compute new terms up to n. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t term = c1 * cache[i-1]          /* + 3461450947505*a(n-1) */
                       + c2 * cache[i-2]          /* + 6440022564931157490*a(n-2) */
                       - c2 * cache[i-3]          /* - 6440022564931157490*a(n-3) */
                       - c1 * cache[i-4]          /* - 3461450947505*a(n-4) */
                       +   cache[i-5];            /* + a(n-5) */
            cache[i] = term;
        }
        cache_len = n;
    }

    return cache[n];
}
