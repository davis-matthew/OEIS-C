/*
 * A062752  Row sums of unsigned N(4) staircase array A062751.
 * https://oeis.org/A062752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   3*n*(3*n-1)*(3*n+1)*a(n) = (2021*n^3-3072*n^2+1411*n-192)*a(n-1)
 *            * + 64*(2*n-1)*(4*n-3)*(4*n-1)*a(n-2), a(0)=1, a(1)=15
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;

    /* Reallocate to hold values up to n */
    seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return;   /* allocation failed – leave cache incomplete */

    /* Compute new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            new_cache[0] = (seq_t)1;
        } else if (i == 1) {
            new_cache[1] = (seq_t)15;
        } else {
            if (sizeof(seq_t) <= 8) {   /* 64‑bit or smaller build */
                unsigned __int128 ii = i;
                /* compute the polynomial part */
                unsigned __int128 num_poly = (unsigned __int128)2021 * ii * ii * ii
                    - (unsigned __int128)3072 * ii * ii
                    + (unsigned __int128)1411 * ii
                    - 192;
                unsigned __int128 term1 = num_poly * (unsigned __int128)new_cache[i - 1];
                unsigned __int128 term2 = (unsigned __int128)64
                    * (unsigned __int128)(2 * ii - 1)
                    * (unsigned __int128)(4 * ii - 3)
                    * (unsigned __int128)(4 * ii - 1)
                    * (unsigned __int128)new_cache[i - 2];
                unsigned __int128 numerator = term1 + term2;
                unsigned __int128 denom = (unsigned __int128)3 * ii
                    * (unsigned __int128)(3 * ii - 1)
                    * (unsigned __int128)(3 * ii + 1);
                new_cache[i] = (seq_t)(numerator / denom);
            } else {                     /* arbitrary‑precision build */
                seq_t iv  = (seq_t)i;
                seq_t iv2 = iv * iv;
                seq_t iv3 = iv2 * iv;
                seq_t num_poly = (seq_t)2021 * iv3
                    - (seq_t)3072 * iv2
                    + (seq_t)1411 * iv
                    - (seq_t)192;
                seq_t term1 = num_poly * new_cache[i - 1];
                seq_t term2 = (seq_t)64 * (seq_t)(2 * iv - 1)
                    * (seq_t)(4 * iv - 3) * (seq_t)(4 * iv - 1)
                    * new_cache[i - 2];
                seq_t numerator = term1 + term2;
                seq_t denom = (seq_t)3 * iv * (seq_t)(3 * iv - 1)
                              * (seq_t)(3 * iv + 1);
                new_cache[i] = numerator / denom;
            }
        }
    }

    cache = new_cache;
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return (n < cache_size) ? cache[n] : (seq_t)0;
}
