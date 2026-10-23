/*
 * A169113  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^26 = I.
 * https://oeis.org/A169113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Generating function (see OEIS A169113) gives a linear recurrence
 *            * of order 26: for n > 26 * a(n) = 2*∑_{k=1}^{25} a(n-k) -
 *            3*a(n-26). * Initial values: * a(0) = 1, * a(n) = 4*3^(n-1) for
 *            1 ≤ n ≤ 25, * a(26) = 2*∑_{k=1}^{25} a(26-k) - 2 (constant term
 *            from the gf)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t   *cache = NULL;   /* memoised sequence values */
static idx_t    computed = 0;   /* number of values computed (size of cache) */

/*
 * Compute a(idx_t n) returning the nth term of A169113.
 * The function may be called in any order; it memoises computed values.
 */
seq_t a(idx_t n)
{
    /* If no cache yet, allocate enough space for at least n+1 values. */
    if (cache == NULL || computed <= n) {
        idx_t needed = n + 1;
        /* Reallocate if cache already exists */
        if (cache != NULL && computed < needed) {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        } else if (cache == NULL) {
            cache = (seq_t *)malloc(needed * sizeof(seq_t));
        }
        /* Compute all missing values up to n (inclusive). */
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i <= 25) {
                if (i == 1)
                    cache[1] = (seq_t)4;
                else
                    cache[i] = cache[i - 1] * (seq_t)3;
            } else if (i == 26) {
                /* a(26) = 2 * sum_{k=1}^{25} a(26-k) - 2 */
                seq_t sum25 = (seq_t)0;
                for (idx_t k = 1; k <= 25; ++k)
                    sum25 += cache[26 - k];
                cache[26] = sum25 * (seq_t)2 - (seq_t)2;
            } else { /* i >= 27 */
                seq_t sum25 = (seq_t)0;
                for (int k = 1; k <= 25; ++k)
                    sum25 += cache[i - k];
                cache[i] = sum25 * (seq_t)2 - cache[i - 26] * (seq_t)3;
            }
        }
        computed = n + 1;   /* we now have values 0 .. n */
    }
    return cache[n];
}
