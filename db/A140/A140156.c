/*
 * A140156  a(1)=1, a(n) = a(n-1) + n^3 if n odd, a(n) = a(n-1) + n^5 if n is
 *          even.
 * https://oeis.org/A140156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(n) = a(n-1) + n^3 if n odd, a(n) = a(n-1) + n^5 if n
 *            even
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;  /* array of computed a(i), 1-indexed stored at i-1 */
    static idx_t cache_cap   = 0;     /* current allocated size */
    static idx_t max_computed = 0;    /* largest n for which a(n) is known */

    if (n == 0) return 0; /* undefined for n<1, but safe fallback */

    /* Initial allocation */
    if (cache == NULL) {
        cache_cap = n;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;      /* out of memory: return 0 */
        cache[0] = 1;              /* a(1) */
        max_computed = 1;
    }

    /* Grow array if needed */
    if (n > cache_cap) {
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        if (!tmp) return 0;        /* out of memory: return 0 */
        cache = tmp;
    }

    /* Compute missing values up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t inc;
            if (i & 1U) { /* odd */
                inc = (seq_t)i * (seq_t)i * (seq_t)i; /* i^3 */
            } else {      /* even */
                seq_t ii  = (seq_t)i * (seq_t)i;          /* i^2 */
                seq_t iii = ii * (seq_t)i;                /* i^3 */
                seq_t i4  = iii * (seq_t)i;               /* i^4 */
                inc      = i4 * (seq_t)i;                 /* i^5 */
            }
            cache[i - 1] = cache[i - 2] + inc;
        }
        max_computed = n;
    }

    return cache[n - 1];
}
