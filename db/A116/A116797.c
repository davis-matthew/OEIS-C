/*
 * A116797  Number of permutations of length n which avoid the patterns 1432,
 *          2134, 2314.
 * https://oeis.org/A116797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence: * a(1) = 1, a(2) = 2, a(3)
 *            = 6, a(4) = 21, a(5) = 73, a(6) = 241, * a(n) = 6*a(n-1) -
 *            13*a(n-2) + 15*a(n-3) - 9*a(n-4) + 7*a(n-5) - 4*a(n-6) * for all
 *            n >= 7. The recurrence is derived from the denominator of the *
 *            generating function for A116797
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage required by the build system */
seq_t a(idx_t n)
{
    /* Store computed values in a static cache to allow non‑monotonic calls */
    static seq_t *cache = NULL;      /* array indexed by 1..n */
    static size_t cache_size = 0;    /* allocated size (elements) */
    static idx_t computed = 0;       /* highest index already computed */

    if (n == (idx_t)0)            /* sequence is 1‑indexed; 0 is undefined */
        return (seq_t)0;

    /* Allocate or extend the cache as needed */
    size_t needed = (size_t)(n + 1);  /* +1 so that index n is valid */
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        cache = tmp;
        cache_size = needed;
    }

    /* Fill initial 6 terms if they have not been set yet */
    for (size_t i = (size_t)computed + 1; i <= 6 && i <= (size_t)n; ++i) {
        switch (i) {
            case 1: cache[i] = (seq_t)1;   break;
            case 2: cache[i] = (seq_t)2;   break;
            case 3: cache[i] = (seq_t)6;   break;
            case 4: cache[i] = (seq_t)21;  break;
            case 5: cache[i] = (seq_t)73;  break;
            case 6: cache[i] = (seq_t)241; break;
        }
    }

    /* Compute remaining terms up to n */
    for (size_t i = (size_t)computed + 1; i <= (size_t)n; ++i) {
        if (i <= 6) continue;  /* already set */

        /* a(i) = 6*a(i-1) - 13*a(i-2) + 15*a(i-3) - 9*a(i-4) + 7*a(i-5) - 4*a(i-6) */
        seq_t val = (seq_t)6 * cache[i-1];
        val -= (seq_t)13 * cache[i-2];
        val += (seq_t)15 * cache[i-3];
        val -= (seq_t)9  * cache[i-4];
        val += (seq_t)7  * cache[i-5];
        val -= (seq_t)4  * cache[i-6];
        cache[i] = val;
    }

    computed = n;
    return cache[n];
}
