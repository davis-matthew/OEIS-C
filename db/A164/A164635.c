/*
 * A164635  Number of reduced words of length n in Coxeter group on 22
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -210*a(n-7) + 20*Σ_{k=1..6} a(n-k) + p(n) * where p(0..7)
 *            = [1,2,2,2,2,2,2,1] (coefficients of the numerator), * p(n)=0
 *            otherwise. * Initial values (offset 0): * a(0)=1, a(1)=22,
 *            a(2)=462, a(3)=9702, * a(4)=203742, a(5)=4278582, a(6)=89850222.
 *            * The function caches computed terms so that s...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t pcoeff[8] = {
    (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)2,
    (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)1
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored (≥ n+1 when called) */

    /* ----- initialize cached array with first 8 terms (a0..a7) ----- */
    if (cache == NULL) {
        cache_size = 8;                 /* indices 0..7 */
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failure, trivial */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)462;
        cache[3] = (seq_t)9702;
        cache[4] = (seq_t)203742;
        cache[5] = (seq_t)4278582;
        cache[6] = (seq_t)89850222;

        /* compute a(7) using the recurrence with pcoeff[7] = 1 */
        seq_t sum06 = (seq_t)0;
        for (idx_t k = 1; k <= 6; ++k)
            sum06 += cache[7 - k];      /* sum of a(1)..a(6) */
        cache[7] = (seq_t)20 * sum06
                  - (seq_t)210 * cache[0]
                  + pcoeff[7];           /* pcoeff[7] == 1 */
    }

    /* ----- grow cache if needed ----- */
    size_t old_size = cache_size;
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure, trivial */
        cache = tmp;
        cache_size = new_size;
    }

    /* ----- compute missing terms up to n ----- */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i <= 6)          /* initial values already set */
            continue;

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 6; ++k)
            sum += cache[i - k];          /* sum of last 6 terms */

        seq_t term = (seq_t)20 * sum
                   - (seq_t)210 * cache[i - 7];

        if (i <= 7)            /* add numerator coefficient if inside range */
            term += pcoeff[i];

        cache[i] = term;
    }

    return cache[n];
}
