/*
 * A189735  a(1)=3, a(2)=1, a(n) = 3*a(n-1) + a(n-2).
 * https://oeis.org/A189735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=1, a(n)=3*a(n-1)+a(n-2) (offset 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed terms */
static idx_t cache_size = 0;     /* number of terms currently stored */

/* Returns the n-th term of A189735.  n >= 1.  */
seq_t a(idx_t n)
{
    if (n == 0) {                /* undefined for offset 1, return 0 as safe default */
        return (seq_t)0;
    }

    /* Expand cache if necessary */
    if (cache_size < n) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!new_cache) { /* allocation failed */
            exit(1);
        }
        cache = new_cache;

        for (idx_t i = cache_size; i < n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)3;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = ((seq_t)3) * cache[i - 1] + cache[i - 2];
            }
        }
        cache_size = n;
    }

    return cache[n - 1];
}
