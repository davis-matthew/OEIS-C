/*
 * A159753  Numerator of Hermite(n, 10/21).
 * https://oeis.org/A159753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 21^n * Hermite(n, 10/21)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 6, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *buf = NULL;
    static size_t cap = 0;           /* currently allocated capacity */
    static idx_t  computed = (idx_t)-1;   /* highest index computed */

    /* Initialise cache with first two values a(0)=1, a(1)=20 */
    if (buf == NULL) {
        cap = 2;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) return (seq_t)0;   /* allocation failure – return zero */
        buf[0] = (seq_t)1;          /* a(0) */
        buf[1] = (seq_t)20;         /* a(1) */
        computed = 1;
    }

    /* Return already computed small indices immediately */
    if (n == (idx_t)0) return buf[0];
    if (n == (idx_t)1) return buf[1];

    /* Grow array if needed */
    if ((size_t)n >= cap) {
        size_t newcap = cap ? cap : 1;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* reallocation failure – return zero */
        buf = tmp;
        cap = newcap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed + (idx_t)1; k <= n; ++k) {
        seq_t term1 = (seq_t)20 * buf[k - 1];
        /* term2 = 882 * (k-1) * buf[k-2] */
        seq_t term2 = (seq_t)882 * (seq_t)(k - 1);
        term2 = term2 * buf[k - 2];
        buf[k] = term1 - term2;
    }
    if ((idx_t)cap > computed) computed = (idx_t)n;
    return buf[n];
}
