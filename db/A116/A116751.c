/*
 * A116751  Number of permutations of length n which avoid the patterns 2314,
 *          2431, 3124.
 * https://oeis.org/A116751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Data: a(n) = 6*a(n-1) - 12*a(n-2) + 13*a(n-3) - 6*a(n-4) + a(n-5)
 *  Initial values (offset 1):
 *      a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=75
 *
 *  The sequence is computed by memoizing a growing table of values.
 *  The implementation uses only integer arithmetic the types seq_t
 *  and idx_t provide.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1 */

    /* static cache structure */
    static seq_t *cache = NULL;
    static idx_t  cap    = 0;  /* capacity of cache (size of array) */
    static idx_t  computed = 0; /* highest n whose value has been computed */

    /* allocate and initialize cache if this is the first call */
    if (computed == 0) {
        cap = 6;                        /* cover indices 1..5 at least */
        cache = seq_alloc(cap);
        /* base values */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)75;
        computed = 5;
    }

    /* grow array if necessary */
    if ((idx_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 6) new_cap = 6;
        cache = seq_realloc(cache, cap, new_cap);
        cap = new_cap;
    }

    /* return cached value if already computed */
    if (n <= computed) return cache[n];

    /* compute missing terms using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] * 6;         /* 6*a(i-1) */
        val -= cache[i - 2] * 12;            /* -12*a(i-2) */
        val += cache[i - 3] * 13;            /* +13*a(i-3) */
        val -= cache[i - 4] * 6;             /* -6*a(i-4)  */
        val += cache[i - 5];                 /* +a(i-5)    */
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
