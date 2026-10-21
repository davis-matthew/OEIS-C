/*
 * A231672  a(n) = Sum_{i=0..n} digsum_6(i), where digsum_6(i) = A053827(i).
 * https://oeis.org/A231672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} digsum_6(i) where digsum_6(i) = digit sum of
 *            i in base 6 (OEIS A053827)
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digsum = NULL;   /* digit sums in base 6 */
    static seq_t *prefix = NULL;   /* cumulative sums of digsum */
    static idx_t capacity = 0;     /* allocated size of arrays */
    static idx_t last = 0;         /* highest index computed (inclusive) */

    if (n == 0)
        return 0;

    /* first time: allocate space for index 0 */
    if (capacity == 0) {
        capacity = 1;
        digsum = (seq_t *)malloc(sizeof(seq_t) * capacity);
        prefix = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!digsum || !prefix) /* allocation failure – abort */
            return 0;
        digsum[0] = 0;
        prefix[0] = 0;
        last = 0;
    }

    /* extend capacity if needed */
    if (n >= capacity) {
        idx_t newcap = capacity * 2;
        if (newcap <= n) newcap = n + 1;
        seq_t *tmpd = (seq_t *)realloc(digsum, sizeof(seq_t) * newcap);
        seq_t *tmpf = (seq_t *)realloc(prefix, sizeof(seq_t) * newcap);
        if (!tmpd || !tmpf) /* allocation failure – abort */
            return 0;
        digsum = tmpd;
        prefix = tmpf;
        capacity = newcap;
    }

    /* compute new values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t d = digsum[i / 6] + (seq_t)(i % 6);
        digsum[i] = d;
        prefix[i] = prefix[i - 1] + d;
    }
    last = n;

    return prefix[n];
}
