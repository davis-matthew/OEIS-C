/*
 * A170707  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G(t) = (1 + 2t + 2t² + … + 2t⁴⁹ + t⁵⁰) / * (1 – 24t – 24t² – … –
 *            24t⁴⁹ + 300t⁵⁰). * Coefficients a(n) satisfy * a(n) = p(n) + 24
 *            * (a(n–1)+…+a(n–49)) – 300 * a(n–50), * where p(0)=1,
 *            p(1)…p(49)=2, p(50)=1, p(n)=0 for n>50
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized sequence values          */
static size_t  cache_size = 0;   /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* Ensure the cache is big enough for the requested index. */
    size_t idx = (size_t)n;
    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) abort();          /* allocation failed */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }
    if (idx >= cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        cache = tmp;

        /* Compute new values starting from the previous size. */
        for (size_t k = cache_size; k <= idx; ++k) {
            seq_t p;                 /* numerator coefficient for a(k) */
            if (k == 0)      p = (seq_t)1;
            else if (k <= 49) p = (seq_t)2;
            else if (k == 50) p = (seq_t)1;
            else              p = (seq_t)0;

            seq_t total = p;

            /* Sum over the last 49 terms (if they exist). */
            for (size_t i = 1; i <= 49; ++i) {
                if (k >= i) {
                    total += (seq_t)24 * cache[k - i];
                } else {
                    break;          /* further terms would be negative indices */
                }
            }

            /* Subtract 300 * a(k-50) if applicable. */
            if (k >= 50) {
                total -= (seq_t)300 * cache[k - 50];
            }

            cache[k] = total;
        }
        cache_size = new_size;
    }
    return cache[idx];
}
