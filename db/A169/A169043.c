/*
 * A169043  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0: a(n)=∑_{j=1}^{min(n,23)} 28·a(n−j) + (n>=24?
 *            –406·a(n−24) : 0) + (n==0||n==24?1 : (n>=1&&n<=23?2:0)); This
 *            follows from G(t)=P(t)/Q(t) with Q(t)=1–28t–…–28t^23+406t^24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated capacity */
    static idx_t max_computed = 0; /* largest index computed so far */

    /* allocate (or create) cache[0] */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
        cap = 1;
    }

    /* return already computed value */
    if (n <= max_computed)
        return cache[n];

    /* grow cache to hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (newp == NULL) abort();
        cache = newp;
        cap = newcap;
    }

    /* compute missing terms up to n inclusively */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t limit = (i <= 24) ? i : 24;

        for (idx_t j = 1; j <= limit; ++j) {
            if (j == 24) {
                /* coefficient -406 */
                sum -= cache[i - 24] * (seq_t)406;
            } else {
                /* coefficient +28 */
                sum += cache[i - j] * (seq_t)28;
            }
        }

        if (i <= 24) {
            if (i == 0 || i == 24)
                sum += (seq_t)1;
            else
                sum += (seq_t)2;          /* for i = 1…23 */
        }

        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
