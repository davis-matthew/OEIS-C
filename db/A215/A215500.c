/*
 * A215500  a(n) = ((sqrt(5) + 3)^n + (-sqrt(5) -1)^n + (-sqrt(5) + 3)^n +
 *          (sqrt(5) - 1)^n) / 2^n.
 * https://oeis.org/A215500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ((sqrt(5)+3)^n + (-sqrt(5)-1)^n + (-sqrt(5)+3)^n + (sqrt(5)-1)^n)/2^n
   recurrence: a(n)=2*a(n-1)+3*a(n-2)-4*a(n-3)+a(n-4)
   a(0)=4, a(1)=2, a(2)=10, a(3)=14 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base cases for small n */
    if (n < 4) {
        switch (n) {
            case 0: return 4;
            case 1: return 2;
            case 2: return 10;
            case 3: return 14;
        }
    }

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0; /* largest index computed, >=3 after init */

    /* allocate or grow cache */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    } else if (cache_cap < (size_t)(n + 1)) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
    }

    /* initialise first four terms if not yet done */
    if (last_computed < 3) {
        cache[0] = 4;
        cache[1] = 2;
        cache[2] = 10;
        cache[3] = 14;
        last_computed = 3;
    }

    /* compute values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* temp1 = 2*a[i-1]  (shift left by 1) */
        seq_t temp1 = cache[i - 1] << 1;
        /* temp2 = 3*a[i-2]  (multiply by 3) */
        seq_t temp2 = cache[i - 2] * 3;
        /* temp3 = a[i-4]   */
        seq_t temp3 = cache[i - 4];
        /* temp = temp1 + temp2 + temp3 */
        seq_t temp = temp1 + temp2 + temp3;
        /* temp4 = 4*a[i-3] (shift left by 2) */
        seq_t temp4 = cache[i - 3] << 2;
        /* a[i] = temp - temp4  (safe: temp >= temp4 for all i) */
        cache[i] = temp - temp4;
    }

    last_computed = n;
    return cache[n];
}
