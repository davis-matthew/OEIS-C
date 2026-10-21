/*
 * A098075  Threefold convolution of A004148 (the RNA secondary structure
 *          numbers) with itself.
 * https://oeis.org/A098075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 15, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*Sum_{k=ceil((n+1)/2)..n} binomial(k, n-k)*binomial(k+2, 3+n-k)/k, a(0)=1
 * Recurrence used for efficient computation (valid for all n >= 4):
 *    n^2*(n+6)*a(n) - n*(2*n+5)*(n+2)*a(n-1)
 *  - (n+1)*(n^2+2*n-16)*a(n-2) - n*(n+2)*(2*n-1)*a(n-3)
 *  + (n-4)*(n+2)^2*a(n-4) = 0
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* array of computed a values */
    static idx_t cache_len = 0;     /* number of elements currently stored */

    /* grow cache to hold n+1 elements if necessary */
    if (n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_arr = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!new_arr) return (seq_t)0; /* allocation failure */

        /* copy existing values if any */
        if (cache) {
            for (idx_t i = 0; i < cache_len; ++i)
                new_arr[i] = cache[i];
            free(cache);
        }
        cache = new_arr;
        cache_len = new_len;

        /* initialize base cases */
        if (cache_len > 0) cache[0] = (seq_t)1;
        if (cache_len > 1) cache[1] = (seq_t)3;
        if (cache_len > 2) cache[2] = (seq_t)6;
        if (cache_len > 3) cache[3] = (seq_t)13;

        /* compute missing terms up to n using the recurrence */
        for (idx_t i = 4; i <= n; ++i) {
            seq_t term1 = (seq_t)i * ((seq_t)2 * i + 5) * (i + 2) * cache[i - 1];
            seq_t term2 = (seq_t)(i + 1) * ((seq_t)i * i + 2 * i - 16) * cache[i - 2];
            seq_t term3 = (seq_t)i * (i + 2) * (2 * i - 1) * cache[i - 3];
            seq_t term4 = (seq_t)(i - 4) * (i + 2) * (i + 2) * cache[i - 4];
            seq_t numerator = term1 + term2 + term3 - term4;
            seq_t denominator = (seq_t)i * (seq_t)i * (i + 6);
            cache[i] = numerator / denominator;
        }
    }

    return cache[n];
}
