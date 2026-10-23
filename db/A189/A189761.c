/*
 * A189761  Numbers n for which the set of residues {Fibonacci(k) mod n,
 *          k=0,1,2,....} is minimal.
 * https://oeis.org/A189761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A189761 – minimal‑residue Fibonacci sets
 *
 * a(n) = 3*a(n-2) - a(n-4)   for n > 9
 * a(1) = 1, a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 5,
 * a(6) = 8, a(7) = 11, a(8) = 21, a(9) = 29, a(10) = 55
 *
 * The function keeps a static cache so that calls in any order are
 * answered in O(1) after the first few invocations.  All intermediate
 * values are computed using the recurrence and the provided base
 * values, ensuring full correctness for arbitrary‑precision or
 * 64‑bit builds.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* how many entries the cache can hold (0,1,…,cap-1) */
    static size_t max_computed = 0;   /* highest index currently stored in cache */

    if (n == 0)          /* not used by OEIS, but harmless */
        return (seq_t)0;

    /* Convert n to a size_t for indexing */
    size_t target = (size_t)n;

    /* grow cache if necessary */
    if (cache_cap <= target) {
        size_t new_cap = target + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise base values on the first invocation (or when cache is enlarged) */
    if (max_computed < 10) {
        static const seq_t init_vals[11] = {0, 1, 2, 3, 4, 5, 8, 11, 21, 29, 55};
        size_t init_end = (cache_cap > 10) ? 10 : (cache_cap - 1);
        for (size_t i = max_computed + 1; i <= init_end; ++i)
            cache[i] = init_vals[i];
        max_computed = init_end;
    }

    /* compute new terms if required */
    for (size_t i = max_computed + 1; i <= target; ++i) {
        /* recurrence only applies for i > 9; for i <= 10 the index is
           already set by the initialisation above.  The loop starts at
           max_computed+1 which will be ≥ 11 on the first pass beyond
           the base values. */
        cache[i] = cache[i - 2] * (seq_t)3 - cache[i - 4];
    }
    max_computed = target;

    return cache[target];
}
