/*
 * A176832  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=5, k=1 and l=-1.
 * https://oeis.org/A176832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(1) = 5; for n >= 2 a(n) = sum_{p=0}^{n-1} a(p) *
 *            a(n-1-p) + (n-1) (OEIS A176832)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t len   = 0;      /* number of cached terms */
    static size_t cap   = 0;      /* allocated capacity */

    size_t target = (size_t)n;

    if (target >= len) {
        /* Ensure sufficient capacity */
        if (cap < target + 1) {
            size_t new_cap = cap ? (cap << 1) : 1;
            while (new_cap < target + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);          /* allocation failure */
            cache = tmp;
            cap   = new_cap;
        }

        /* Compute missing terms */
        for (size_t i = len; i <= target; ++i) {
            seq_t val;
            if (i == 0) {
                val = 1;
            } else if (i == 1) {
                val = 5;
            } else {
                seq_t sum = 0;
                size_t m   = i - 1;                /* index of the convolution sum */
                size_t half = m / 2;
                for (size_t p = 0; p <= half; ++p) {
                    seq_t prod = cache[p] * cache[m - p];
                    if (p == m - p) {
                        sum += prod;
                    } else {
                        sum += prod;   /* first occurrence */
                        sum += prod;   /* second occurrence (symmetry) */
                    }
                }
                sum += (seq_t)(i - 1);            /* add (n-1) term */
                val = sum;
            }
            cache[i] = val;
        }

        len = target + 1;
    }

    return cache[n];
}
