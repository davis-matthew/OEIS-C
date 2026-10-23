/*
 * A020767  Product_{k=1..n} b(k), where b(k) = binary expansion of k
 *          (A007088) but read as if it were a decimal number.
 * https://oeis.org/A020767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∏_{k=1..n} (binary digits of k read as a decimal number)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] … cache[k] contain a(0)…a(k)  */
    static size_t allocated = 0;     /* number of cache slots currently allocated */
    static size_t computed = 0;      /* highest index computed + 1 */

    /* initialise cache[0] if not yet done */
    if (allocated == 0) {
        allocated = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * allocated);
        if (cache == NULL) return 0;      /* allocation failure – unlikely */
        cache[0] = 1;
        computed = 1;
    }

    /* grow cache if we need more space */
    if ((size_t)n + 1 > allocated) {
        size_t newsize = allocated * 2;
        if (newsize < (size_t)n + 1) newsize = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * newsize);
        if (tmp == NULL) return cache[computed - 1]; /* graceful fallback */
        cache = tmp;
        allocated = newsize;
    }

    /* compute missing terms */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        idx_t k = (idx_t)i;
        seq_t b = 0;                         /* decimal value of binary(k) */

        /* find most significant set bit in k */
        int msb = -1;
        idx_t t = k;
        while (t > 0) {
            ++msb;
            t >>= 1;
        }

        /* build decimal value from binary bits */
        for (int bit = msb; bit >= 0; --bit) {
            int bitval = (k >> bit) & 1U;
            b = b * 10 + bitval;
        }

        /* a(i) = a(i‑1) * b(k)  (here i == k) */
        cache[i] = cache[i - 1] * b;
    }

    computed = (size_t)n + 1;
    return cache[n];
}
