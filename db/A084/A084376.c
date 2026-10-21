/*
 * A084376  G.f.: (1+x)/Product_{m>0} (1 - x^m).
 * https://oeis.org/A084376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) + A000041(n-1) for n > 0, a(0) = 1. A000041 is
 *            the partition number p(n). The implementation uses the
 *            pentagonal‑number recurrence and memoises p(k) for all k up to
 *            the maximum n seen so far
 * Range:     u8 n <= 14, u16 n <= 39, u32 n <= 121, u64 n <= 405,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache for partition numbers */
    static seq_t *p_cache   = NULL;   /* array of p(k) */
    static idx_t  p_len     = 0;      /* current size of the cache */

    /* --- extend cache to include n --- */
    if (n >= p_len) {
        size_t newlen = (size_t)(n + 1);

        seq_t *tmp = (seq_t *)realloc(p_cache, newlen * sizeof(seq_t));
        if (!tmp) exit(1);            /* out of memory */
        p_cache = tmp;

        /* initialise p(0) = 1 if this is the first extension */
        if (p_len == 0) {
            p_cache[0] = 1;
            p_len = 1;
        }

        /* compute p(k) for all k from p_len to n */
        for (idx_t k = p_len; k <= n; ++k) {
            seq_t pos = 0;   /* sum of terms with positive sign */
            seq_t neg = 0;   /* sum of terms with negative sign */

            for (idx_t i = 1; ; ++i) {
                /* generalized pentagonal numbers */
                idx_t g1 = i * (3 * i - 1) / 2;
                if (g1 > k) break;   /* no further terms */

                if (i & 1)    /* odd i -> positive sign */
                    pos += p_cache[k - g1];
                else          /* even i -> negative sign */
                    neg += p_cache[k - g1];

                idx_t g2 = i * (3 * i + 1) / 2;
                if (g2 > k) continue;  /* skip if beyond k */

                if (i & 1)    /* same sign for g2 */
                    pos += p_cache[k - g2];
                else
                    neg += p_cache[k - g2];
            }

            /* for k > 0, pos >= neg, so subtraction is safe */
            p_cache[k] = pos - neg;
        }

        p_len = newlen;
    }

    /* --- return a(n) --- */
    if (n == 0)
        return p_cache[0];
    return p_cache[n] + p_cache[n - 1];
}
