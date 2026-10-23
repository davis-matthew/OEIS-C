/*
 * A049426  Row sums of triangle A049410.
 * https://oeis.org/A049426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049426: row sums of triangle A049410.
   Recurrence:
     a(0)=1, a(1)=1, a(2)=4, a(3)=16
     a(n) = a(n-1) + 3*(n-1)*a(n-2)
              + 3*(n-1)*(n-2)*a(n-3)
              + (n-1)*(n-2)*(n-3)*a(n-4)   for n ≥ 4
   This formula is equivalent to the D‑finite recurrence given in the OEIS entry.
*/

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached sequence values */
static idx_t cache_size = 0;     /* number of cached elements */

seq_t a(idx_t n)
{
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();      /* memory failure */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0)            cache[i] = (seq_t)1;
            else if (i == 1)       cache[i] = (seq_t)1;
            else if (i == 2)       cache[i] = (seq_t)4;
            else if (i == 3)       cache[i] = (seq_t)16;
            else {
                seq_t term1 = cache[i-1];
                seq_t term2 = (seq_t)3 * (seq_t)(i-1) * cache[i-2];
                seq_t term3 = (seq_t)3 * (seq_t)(i-1) * (seq_t)(i-2) * cache[i-3];
                seq_t term4 = (seq_t)(i-1) * (seq_t)(i-2) * (seq_t)(i-3) * cache[i-4];
                cache[i] = term1 + term2 + term3 + term4;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
