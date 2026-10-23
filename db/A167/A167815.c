/*
 * A167815  Number of reduced words of length n in Coxeter group on 36
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sequence defined by the rational generating function G(t) =
 *            (t^15 + 2*t^14 + … + 2*t + 1)/(595*t^15 - 34*t^14 - … - 34*t +
 *            1). The coefficients satisfy the linear recurrence a(n) = 34
 *            a(n−1) + … + 34 a(n−14) – 595 a(n−15) for n ≥ 15 with initial
 *            values a(0) = 1, a(n) = 2 + 34·(a(0)+…+a(n−1)) for...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;     /* capacity of cache */
    static size_t len   = 0;     /* number of terms stored (next index = len) */

    /* coefficients of the denominator: 1 - 34 t - … - 34 t^14 + 595 t^15   */
    /* stored as b[1..15] with b[15] = -595                                                     */
    static const int64_t b[16] = {
        0,      /* unused */
        34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34,   /* b[1]..b[14] */
       -595            /* b[15]                                                   */
    };

    /* initialise cache if it does not exist yet */
    if (cache == NULL) {
        /* at least 16 entries are needed (a[0]..a[15])                  */
        cap = 16;
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */

        /* compute the initial 16 terms using the partial recurrence   */
        seq_t sum = 0;                /* sum of a[0]..a[i-1]                      */

        /* a[0] */
        cache[0]  = (seq_t)1;
        sum = cache[0];

        /* a[1] .. a[14] */
        for (idx_t i = 1; i <= 14; ++i) {
            cache[i] = (seq_t)2 + (seq_t)34 * sum;
            sum += cache[i];
        }

        /* a[15] : use the full recurrence (coefficient of t^15)      */
        /* a[15] = 1 + 34 Σ_{j=1}^{14} a[j] – 595 a[0]                */
        seq_t total = (seq_t)1;
        for (idx_t j = 1; j <= 14; ++j)
            total += (seq_t)34 * cache[j];
        total -= (seq_t)595 * cache[0];
        cache[15] = total;

        /* we have computed terms 0 .. 15 */
        len = 16;
    }

    /* enlarge cache if necessary */
    if (n >= (idx_t)cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    while (len <= (size_t)n) {
        seq_t sum14 = 0;                         /* Σ_{k=1}^{14} a[len‑k]   */

        for (int k = 1; k <= 14; ++k)
            sum14 += cache[len - k];

        /* a[len] = 34 * sum14 – 595 * a[len‑15]   */
        cache[len] = (seq_t)34 * sum14 - (seq_t)595 * cache[len - 15];
        ++len;
    }

    return cache[n];
}
