/*
 * A172285  a(n) = (5*2^n - 5*(-1)^n - 3*n*(-1)^n) / 9.
 * https://oeis.org/A172285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*2^n - 5*(-1)^n - 3*n*(-1)^n)/9 Recurrence: a(n) =
 *            3*a(n-2) + 2*a(n-3), n>2, a(0)=0, a(1)=2, a(2)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t last = (idx_t)-1;   /* Last index for which cache[n] is valid */

seq_t a(idx_t n)
{
    /* Initialise cache with the first three terms if not already done */
    if (cache == NULL) {
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 1;
        last = 2;
    }

    /* Return cached value if already computed */
    if (n <= last) {
        return cache[n];
    }

    /* Extend cache to hold up to index n */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute missing values using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 3 * cache[i - 2] + 2 * cache[i - 3];
    }
    last = n;
    return cache[n];
}
