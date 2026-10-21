/*
 * A225032  Non-crossing, non-nesting, 6-colored set partitions.
 * https://oeis.org/A225032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 63*a(n-1) - 1589*a(n-2) + 20515*a(n-3) - 142915*a(n-4) +
 *            509549*a(n-5) - 727767*a(n-6) + a(n-7) a(0) = 1, a(1) = 7, a(2)
 *            = 55, a(3) = 493, a(4) = 5029, a(5) = 57379, a(6) = 716443
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute a(n) as defined in OEIS A225032 */
seq_t a(idx_t n)
{
    /* cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap  = 0;   /* allocated size */
    static size_t len  = 0;   /* number of values currently stored (next free) */

    /* return well‑known first seven terms for n < 7 */
    if (n < 7) {
        static const seq_t init[7] = {
            (seq_t)1, (seq_t)7, (seq_t)55, (seq_t)493,
            (seq_t)5029, (seq_t)57379, (seq_t)716443
        };
        return init[(size_t)n];
    }

    /* initialise cache on first use */
    if (cache == NULL) {
        /* start with a modest capacity */
        cap = 64;
        if (cap < 8) cap = 8;
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure – abort */

        /* store the first seven values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)55;
        cache[3] = (seq_t)493;
        cache[4] = (seq_t)5029;
        cache[5] = (seq_t)57379;
        cache[6] = (seq_t)716443;
        len = 7;
    }

    /* grow the array if necessary */
    if (len <= (size_t)n) {
        size_t newcap = cap;
        while (newcap <= (size_t)n) {
            newcap = newcap * 2;
            if (newcap < 8) newcap = 8;
        }
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure – abort */
        cache = tmp;
        cap   = newcap;
        /* initialise new elements to zero */
        for (size_t i = len; i < newcap; ++i)
            cache[i] = (seq_t)0;
    }

    /* compute missing values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t pos = cache[i-1] * (seq_t)63
                  + cache[i-3] * (seq_t)20515
                  + cache[i-5] * (seq_t)509549
                  + cache[i-7];
        seq_t neg = cache[i-2] * (seq_t)1589
                  + cache[i-4] * (seq_t)142915
                  + cache[i-6] * (seq_t)727767;
        cache[i] = pos - neg;       /* pos >= neg for all n >= 7 */
    }
    len = (size_t)n + 1;
    return cache[(size_t)n];
}
