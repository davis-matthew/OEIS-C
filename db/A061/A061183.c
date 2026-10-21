/*
 * A061183  One-fourth of the fourth (m=3) column of triangle A060921
 *          (bisection of Fibonacci triangle, odd part).
 * https://oeis.org/A061183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A060921(n+3,3)/4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function
   ((1-x^3)-2*(x-x^2))/(1-3*x+x^2)^4.  */
/* For n ≥ 0:  a[n] + Σ_{k=1..8} c[k]·a[n-k] = r[n],  where   */
/*   c[0..8] = {1, -12, 58, -144, 195, -144, 58, -12, 1}             */
/*   r[0..3] = {1, -2, 2, -1} , r[n]=0 for n≥4.                      */
/*   a[n-k] is taken as 0 if n-k<0.                                 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */

static const int _a061183_c[9] = {
    1,   /* c0 */
   -12,   /* c1 */
    58,   /* c2 */
   -144,   /* c3 */
    195,   /* c4 */
   -144,   /* c5 */
    58,   /* c6 */
   -12,   /* c7 */
     1    /* c8 */
};

static seq_t * _a061183_cache = NULL;   /* array of computed terms */
static size_t  _a061183_cache_size = 0; /* allocated size of array    */
static size_t  _a061183_cache_up_to = 0;/* first index not yet computed */

/* Ensure that the cache can hold indices up to n. */
static void _ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > _a061183_cache_size) {
        size_t new_size = needed > (_a061183_cache_size * 2) ?
                          needed : (_a061183_cache_size * 2);
        seq_t *ptr = (seq_t *)realloc(_a061183_cache,
                                      new_size * sizeof(seq_t));
        if (!ptr) {
            /* In a real system we would handle OOM, here we abort. */
            exit(1);
        }
        _a061183_cache = ptr;
        _a061183_cache_size = new_size;
    }
}

/* Public sequence accessor */
seq_t a(idx_t n)
{
    /* Grow cache enough to compute up to n. */
    _ensure_cache(n);

    /* Compute missing terms. */
    for (size_t i = _a061183_cache_up_to; i <= (size_t)n; ++i) {
        seq_t val = 0;

        /* Convolution with the denominator coefficients c[1..8]. */
        for (int k = 1; k <= 8; ++k) {
            if (i < (size_t)k) continue;        /* a[i-k] = 0 for negative index */
            int ck = _a061183_c[k];
            if (ck == 0) continue;
            seq_t term = _a061183_cache[i - k];
            if (ck > 0) {          /* subtract positive contribution */
                val -= (seq_t)ck * term;
            } else {               /* add negative contribution */
                val += (seq_t)(-ck) * term;
            }
        }

        /* Add the numerator term r[i] for i < 4. */
        if (i < 4) {
            int rn;
            switch (i) {
                case 0: rn =  1; break;
                case 1: rn = -2; break;
                case 2: rn =  2; break;
                default: rn = -1; /* i==3 */
            }
            if (rn > 0) {
                val += (seq_t)rn;
            } else {
                val -= (seq_t)(-rn);
            }
        }

        _a061183_cache[i] = val;
    }

    _a061183_cache_up_to = (size_t)n + 1;
    return _a061183_cache[(size_t)n];
}
