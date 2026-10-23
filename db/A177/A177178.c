/*
 * A177178  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=8, k=1 and l=-1.
 * https://oeis.org/A177178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n+1) = sum_{p=0}^{n} a(p) * a(n-p) + n (k=1, l=-1), with a(0)
 *            = 1, a(1) = 8
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms currently initialized */

    /* Ensure the cache can hold terms up to n */
    if (n >= cache_len) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;

        /* Initialize missing initial terms if needed */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
        }
        if (new_cap > 1 && cache_len < 2) {
            cache[1] = (seq_t)8;
        }

        /* Compute all missing terms up to n */
        for (idx_t m = cache_len; m <= n; ++m) {
            if (m == 0 || m == 1) {
                /* a(0) and a(1) are already known */
                continue;
            }

            seq_t sum = (seq_t)0;
            idx_t len = m - 1;            /* n in the recurrence */
            idx_t mid = len >> 1;         /* floor(len / 2) */

            for (idx_t p = 0; p <= mid; ++p) {
                idx_t q = len - p;
                seq_t prod = cache[p] * cache[q];
                if (p < q) {
                    sum += prod;
                    sum += prod;          /* double for symmetric pair */
                } else {                  /* p == q */
                    sum += prod;
                }
            }

            cache[m] = sum + (seq_t)(len);   /* add the "+n" term */
        }

        cache_len = n + 1;
    }

    return cache[n];
}
