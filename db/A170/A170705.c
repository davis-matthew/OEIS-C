/*
 * A170705  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 22 * Σ_{k=1}^{min(49,n)} a(n-k) - 253 * a(n-50)
 *            + b(n) where b(0)=1, b(n)=2 for 1≤n≤49, b(50)=1, and b(n)=0
 *            otherwise
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  len   = 0;  /* number of elements already filled */
    static size_t cap   = 0;  /* allocated capacity of cache */

    /* expand cache if needed */
    if ((size_t)n + 1 > cap) {
        size_t newcap = 128;
        while (newcap <= (size_t)n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise first element a(0) = 1 */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* quick return if already computed */
    if (n < len)
        return cache[n];

    /* compute missing terms up to n inclusive */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;
        idx_t up = (i < 50 ? i : 49);   /* number of previous terms to sum */
        for (idx_t k = 1; k <= up; ++k)
            sum += cache[i - k];

        seq_t val = sum * (seq_t)22;

        /* add the b(i) term from the numerator */
        if (i <= 49)
            val += (seq_t)2;   /* b(i)=2 for 1≤i≤49 (b(0)=1 handled separately) */
        else if (i == 50)
            val += (seq_t)1;   /* b(50)=1 */
        /* else i>50: b(i)=0 */

        /* subtract the -253*a(i-50) term for i≥50 */
        if (i >= 50)
            val -= cache[i - 50] * (seq_t)253;

        cache[i] = val;
    }

    len = n + 1;
    return cache[n];
}
