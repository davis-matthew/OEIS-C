/*
 * A170546  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^47 = I.
 * https://oeis.org/A170546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the rational generating function
   of A170546.

   Let D(t)=1-7t-7t^2-…-7t^46+28t^47   (degree 47)
   Let N(t)=1+2t+2t^2+…+2t^46+t^47

   Then a(0)=1 and for n>0

   a(n) = nN(n) - Σ_{k=1}^{min(n,47)} d_k * a(n-k)

   which gives the following closed recurrences:
     a(0) = 1
     a(n) = 2 + 7 Σ_{k=1}^{n} a(n-k)          (1 ≤ n ≤ 46)
     a(47) = 1 + 7 Σ_{k=1}^{46} a(47-k) - 28 a(0)
     a(n) = 7 Σ_{k=1}^{46} a(n-k) - 28 a(n-47) (n > 47)
*/

#include <stdlib.h>
#include <stdbool.h>

/* Computes the n-th term of A170546.
   The function is memoised and works for arbitrary orders of calls. */
seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of elements stored (indices 0..size-1) */

    /* Ensure the cache is long enough and compute missing terms. */
    if (cache == NULL) {
        /* first call: allocate and compute up to n */
        cache_size = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* compute all values 0 .. n */
        for (idx_t i = 0; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i <= 46) {
                seq_t s = 0;
                for (idx_t k = 1; k <= i; ++k) s += cache[i - k];
                cache[i] = (seq_t)2 + ((seq_t)7) * s;
            } else if (i == 47) {
                seq_t s = 0;
                for (idx_t k = 1; k <= 46; ++k) s += cache[47 - k];
                cache[47] = (seq_t)1 + ((seq_t)7) * s - ((seq_t)28) * cache[0];
            } else { /* i > 47 */
                seq_t s = 0;
                for (idx_t k = 1; k <= 46; ++k) s += cache[i - k];
                cache[i] = ((seq_t)7) * s - ((seq_t)28) * cache[i - 47];
            }
        }
    } else if ((idx_t)n >= cache_size) {
        /* extend cache and fill missing terms */
        size_t old_size = cache_size;
        cache_size = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        for (idx_t i = (idx_t)old_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i <= 46) {
                seq_t s = 0;
                for (idx_t k = 1; k <= i; ++k) s += cache[i - k];
                cache[i] = (seq_t)2 + ((seq_t)7) * s;
            } else if (i == 47) {
                seq_t s = 0;
                for (idx_t k = 1; k <= 46; ++k) s += cache[47 - k];
                cache[47] = (seq_t)1 + ((seq_t)7) * s - ((seq_t)28) * cache[0];
            } else { /* i > 47 */
                seq_t s = 0;
                for (idx_t k = 1; k <= 46; ++k) s += cache[i - k];
                cache[i] = ((seq_t)7) * s - ((seq_t)28) * cache[i - 47];
            }
        }
    }

    return cache[n];
}
