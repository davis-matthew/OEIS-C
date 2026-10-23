/*
 * A168848  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^20 = I.
 * https://oeis.org/A168848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(n) + 25·∑_{j=1}^{19} a(n-j) – 325·a(n-20) * where
 *            p(0)=1, p(20)=1, p(1)…p(19)=2, and p(n)=0 for n>20. * a(k)=0 for
 *            k<0. This reproduces the OEIS sequence A168848
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* array of a(0), a(1), ... */
    static size_t  size  = 0;     /* number of elements already stored */

    /* Return cached value if already available. */
    if (n < size)
        return cache[(size_t)n];

    /* Allocate space up to n inclusive. */
    size_t needed = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)
        exit(1);                /* allocation failure */
    cache = tmp;

    /* Compute all missing terms. */
    for (size_t i = size; i < needed; ++i) {
        size_t idx = i;          /* idx fits into size_t for indices up to needed */
        seq_t term = 0;

        /* 25·∑_{j=1}^{19} a(idx-j)   (indices < 0 contribute 0 automatically) */
        for (size_t j = 1; j <= 19; ++j) {
            if (idx >= j)
                term += cache[idx - j] * 25;
        }

        /* Add the numerator coefficient p(idx). */
        if (idx == 0 || idx == 20)
            term += 1;
        else if (idx <= 19)
            term += 2;

        /* Subtract 325·a(idx-20) if applicable. */
        if (idx >= 20)
            term -= cache[idx - 20] * 325;

        cache[i] = term;
    }

    size = needed;
    return cache[(size_t)n];
}
