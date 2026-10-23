/*
 * A116741  Number of permutations of length n which avoid the patterns 1342,
 *          2314, 4213.
 * https://oeis.org/A116741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116741 (permutation pattern avoidance)
   Recurrence: a(n) = 10*a(n-1) - 40*a(n-2) + 82*a(n-3) - 92*a(n-4) + 56*a(n-5)
                - 17*a(n-6) + 2*a(n-7),
   with initial terms a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73, a(6)=241, a(7)=757.  */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* static cache of computed values, 1‑based index; cache[0] is unused */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;     /* allocated capacity (>= n+1) */
    static size_t computed_last = 0; /* highest n whose value is known */

    /* Ensure cache large enough */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure, return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base values if not already done */
    if (computed_last < 7) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 21;
        cache[5] = 73;
        cache[6] = 241;
        cache[7] = 757;
        computed_last = 7;
    }

    /* If requested value already computed, return it */
    if (n <= computed_last) return cache[n];

    /* Compute successive terms up to n */
    for (idx_t i = computed_last + 1; i <= n; ++i) {
        seq_t term = 0;
        /* Sum of positive contributions first to avoid unsigned underflow */
        term = mul_small(cache[i - 1], 10);
        term += mul_small(cache[i - 3], 82);
        term += mul_small(cache[i - 5], 56);
        term += mul_small(cache[i - 7], 2);

        /* Subtract negative contributions */
        term -= mul_small(cache[i - 2], 40);
        term -= mul_small(cache[i - 4], 92);
        term -= mul_small(cache[i - 6], 17);

        cache[i] = term;
    }

    computed_last = n;
    return cache[n];
}
