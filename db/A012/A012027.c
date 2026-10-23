/*
 * A012027  E.g.f. cosh(sin(arctan(x))) = cosh(x/sqrt(1+x^2)) (even powers
 *          only).
 * https://oeis.org/A012027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is defined by the D-finite recurrence a(0)=1, a(1)=1,
 *            a(2)=‑11; for n≥3: a(n) = –(12*n^2 – 24*n + 11)*a(n‑1)
 *            –12*(n‑2)*(n‑1)*(2*n‑3)^2 * a(n‑2)
 *            –16*(n‑3)*(n‑2)^2*(n‑1)*(2*n‑5)*(2*n‑3) * a(n‑3)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are supplied by the build environment. */

/* Memoisation cache. */
static seq_t* cache = NULL;
static size_t cache_size = 0;   /* number of elements currently allocated */

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure the buffer can hold indices 0 … n */
    size_t new_size = (size_t)(n + 1);
    size_t old_size = cache_size;               /* old capacity */
    if (new_size > cache_size) {
        seq_t* tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {                     /* allocation failure – abort */
            exit(1);
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute all missing values up to n */
    for (idx_t i = (idx_t)old_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)-11;
        } else {
            /* cast index to seq_t for arithmetic */
            seq_t nt = (seq_t)i;

            /* (2*n‑3) and its square */
            seq_t t1  = (seq_t)2 * nt - (seq_t)3;
            seq_t t1sq = t1 * t1;

            /* coefficient for a(n‑1) */
            seq_t coeff1 = (seq_t)12 * nt * nt
                         - (seq_t)24 * nt
                         + (seq_t)11;

            /* coefficient for a(n‑2) */
            seq_t c2 = (seq_t)12 * (nt - (seq_t)2)
                       * (nt - (seq_t)1)
                       * t1sq;

            /* coefficient for a(n‑3) */
            seq_t t2 = (seq_t)2 * nt - (seq_t)5;
            seq_t c3 = (seq_t)16 * (nt - (seq_t)3)
                       * (nt - (seq_t)2)
                       * (nt - (seq_t)2)
                       * (nt - (seq_t)1)
                       * t2
                       * t1;

            seq_t term1 = - coeff1 * cache[i - 1];
            seq_t term2 = - c2       * cache[i - 2];
            seq_t term3 = - c3       * cache[i - 3];

            cache[i] = term1 + term2 + term3;
        }
    }

    /* Return the requested value */
    return cache[n];
}
