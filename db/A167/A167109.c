/*
 * A167109  Number of reduced words of length n in Coxeter group on 8
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = p(n) +
 *            6*Σ_{k=1}^{min(13,n)} a(n-k) – 21*a(n-14) with p(0)=1 , p(14)=1
 *            , p(1…13)=2 , p(n>14)=0 (the coefficients come from the rational
 *            generating function given in the OEIS entry). This recursion
 *            yields the sequence A167109 for all n≥0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  seq_t and idx_t are defined by the build.               */
seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache   = NULL;   /* array holding a(0)…a(k) */
    static size_t  len    = 0;      /* number of entries currently stored */
    static size_t  cap    = 0;      /* allocated capacity */

    /* Convert indexed value to size_t for array access (n≤10000 so safe) */
    size_t N = (size_t)n;

    /* Allocate or grow cache if necessary.  5 initial terms are enough to
       start the recurrence (a(0) to a(4) are needed for n≤4).  */
    if (cache == NULL) {
        cap = 16;                     /* start small |
                                            will grow quickly as needed   */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure */
        len = 0;
    }
    if (N >= cap) {
        size_t newcap = cap;
        while (N >= newcap) newcap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms up to n (inclusive) if not already present */
    for (size_t i = len; i <= N; ++i) {
        /* p(i) – the inhomogeneous part */
        seq_t p;
        if (i == 0 || i == 14)
            p = (seq_t)1;
        else if (i < 14)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        /* sum = 6*Σ_{k=1}^{min(13,i)} a(i-k) – 21*a(i-14) (if i≥14)   */
        seq_t sum = (seq_t)0;
        for (size_t k = 1; k <= 13 && k <= i; ++k)
            sum += cache[i - k] * (seq_t)6;
        if (i >= 14)
            sum -= cache[i - 14] * (seq_t)21;

        /* recurrence: a(i) = p(i) + sum */
        cache[i] = p + sum;
    }

    len = N + 1;                /* we now have a(0)…a(n) */
    return cache[N];
}
