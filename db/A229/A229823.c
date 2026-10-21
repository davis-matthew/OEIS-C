/*
 * A229823  Even bisection gives sequence a itself, n->a(2*(8*n+k)-1) gives
 *          k-th differences of a for k=1..8 with a(n)=n for n<2.
 * https://oeis.org/A229823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, eigen
 * Formula:   a(n) = n for n < 2 a(2*n) = a(n) a(2*(8*n + k) - 1) = Σ_{j=0..k}
 *            (-1)^j * C(k,j) * a(n + k - j) for k = 1..8
 * Range:     i8 n <= 46, i16 n <= 1710, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* binomial coefficients up to 8 */
static const int binom[9][9] = {
    {1},
    {1,1},
    {1,2,1},
    {1,3,3,1},
    {1,4,6,4,1},
    {1,5,10,10,5,1},
    {1,6,15,20,15,6,1},
    {1,7,21,35,35,21,7,1},
    {1,8,28,56,70,56,28,8,1}
};

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        /* safe cast of realloc result for both C and C++ */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);      /* allocation failed */
        cache = tmp;

        /* compute values from old size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 2) {
                cache[i] = (seq_t)i;         /* a(0)=0, a(1)=1 */
            } else if ((i & 1) == 0) {
                cache[i] = cache[i >> 1];    /* a(2*n)=a(n) */
            } else {
                /* odd index: i = 2*(8*m + k) - 1  (k in 1..8) */
                idx_t t = (i + 1) >> 1;      /* t = (i+1)/2 */
                idx_t r = t & 7;             /* r = t mod 8 */
                if (r == 0) r = 8;
                idx_t k = r;                 /* k for recurrence */
                idx_t m = (t - k) >> 3;       /* m = (t - k)/8 */

                seq_t sum = 0;
                for (idx_t j = 0; j <= k; ++j) {
                    seq_t term = cache[m + k - j];
                    if (j & 1) term = -term;                  /* (-1)^j */
                    term *= (seq_t)binom[k][j];              /* * C(k,j) */
                    sum += term;
                }
                cache[i] = sum;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
