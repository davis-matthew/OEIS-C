/*
 * A091561  Expansion of (1-2x-sqrt(1-4x+4x^2-4x^3))/(2x^2).
 * https://oeis.org/A091561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=4 * For n ≥ 4: a(n) = ( 2*(2n+1)*a(n-1) -
 *            4*(n-1)*a(n-2) + 2*(2n-5)*a(n-3) ) / (n+2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The recurrence follows from the D‑finite relation:
 *  (n+2)a(n) - 2(2n+1)a(n-1) + 4(n-1)a(n-2) + 2(5-2n)a(n-3)=0
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to allow repeated calls in any order */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;         /* number of allocated slots (1‑based)         */
    static idx_t last_computed = 0;      /* largest n we have computed so far          */

    if (n == 0) return 0;               /* defined value for out of range (not used)  */

    /* ensure at least 4 slots for base values */
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();
        /* we will use indices 1..n inclusive, ignore cache[0] */
        cache[0] = 0;
        cache[1] = (seq_t)1;            /* a(1) */
        cache[2] = (seq_t)2;            /* a(2) */
        cache[3] = (seq_t)4;            /* a(3) */
        last_computed = 3;
    }

    /* enlarge cache if needed */
    if (n >= (idx_t)cache_size) {
        idx_t new_size = (cache_size < 16) ? 16 : cache_size * 2;
        while (new_size <= n) new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 3) continue;          /* already initialized */
        idx_t two_i_plus_one = 2*i + 1;
        idx_t two_i_minus_five = 2*i - 5;

        /* term1 = 2*(2i+1)*a(i-1)  */
        seq_t term1 = cache[i-1] * ((seq_t)two_i_plus_one);
        term1 *= (seq_t)2;

        /* term2 = 4*(i-1)*a(i-2)   */
        seq_t term2 = cache[i-2] * ((seq_t)(i-1));
        term2 *= (seq_t)4;

        /* term3 = 2*(2i-5)*a(i-3)  */
        seq_t term3 = cache[i-3] * ((seq_t)two_i_minus_five);
        term3 *= (seq_t)2;

        seq_t numerator = term1 - term2 + term3;
        cache[i] = numerator / ((seq_t)(i + 2));
    }
    last_computed = n;
    return cache[n];
}
