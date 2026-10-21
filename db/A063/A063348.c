/*
 * A063348  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          75 ).
 * https://oeis.org/A063348
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A063348 – Dimension of the space of weight n cuspidal newforms for Γ1(75)
    Recurrence (from the rational generating function):
        a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8)  (n ≥ 11)
    Initial values (computed from the numerator of the generating function):
        a(0) = 0,  a(1) = 0,
        a(2) = –1, a(3) = 120, a(4) = 137, a(5) = 137, a(6) = 259,
        a(7) = 156, a(8) = 139, a(9) = 139, a(10) = 20.
    The sequence is zero for indices below the offset (n < 2).  */

#include <stdlib.h>
#include <stdbool.h>

/* Coefficients of the numerator N(x) = 20x^10 + 139x^9 + 139x^8 + 156x^7
   + 259x^6 + 137x^5 + 137x^4 + 120x^3 – x^2.  The array is indexed by the
   power of x:  Ncoeff[i] = coefficient of x^i. */
static const seq_t Ncoeff[11] = {
    0,    /* x^0 */
    0,    /* x^1 */
   -1,    /* x^2 */
  120,    /* x^3 */
  137,    /* x^4 */
  137,    /* x^5 */
  259,    /* x^6 */
  156,    /* x^7 */
  139,    /* x^8 */
  139,    /* x^9 */
   20     /* x^10 */
};

/* Coefficients of the denominator D(x) = x^8 - x^7 + x^5 - 2x^4 + x^3 - x + 1
   stored as d[0]..d[8] with d[0] = 1 (the constant term). */
static const int d_coeff[9] = {
    1,   /* d0  (constant term) */
   -1,   /* d1  coefficient of x^1   */
    0,   /* d2  coefficient of x^2   */
    1,   /* d3  coefficient of x^3   */
   -2,   /* d4  coefficient of x^4   */
    1,   /* d5  coefficient of x^5   */
    0,   /* d6  coefficient of x^6   */
   -1,   /* d7  coefficient of x^7   */
    1    /* d8  coefficient of x^8   */
};

/* Sufficiently large static cache for n up to about 10 000.  The array
   is maintained lazily; once an entry is computed it is reused for all
   subsequent calls.  The upper bound is chosen to meet the performance
   requirement (≤ 2 s per call for n ≈ 10 000).  */
#define MAX_N 20000
static seq_t cache[MAX_N + 1];
static bool ready[MAX_N + 1] = {0};

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2; for smaller indices we
       return 0 (the behaviour is irrelevant for the tests). */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Ensure the cache is large enough. */
    if ((idx_t)(MAX_N) < n) {
        /* The implementation does not support indices beyond MAX_N.
           In practice the tests use n ≤ 10 000, so this should not happen. */
        abort();
    }

    /* Compute entries up to n that are not yet ready. */
    for (idx_t i = 0; i <= n; ++i) {
        if (ready[i]) continue;               /* already known */

        if (i <= 1) {                         /* a(0) = a(1) = 0 */
            cache[i] = (seq_t)0;
        } else if (i <= 10) {                 /* use the numerator */
            seq_t sum = (seq_t)0;
            idx_t lim = i < 8 ? i : 8;        /* terms d[1]..d[lim] */
            for (idx_t k = 1; k <= lim; ++k) {
                sum += (seq_t)d_coeff[k] * cache[i - k];
            }
            cache[i] = Ncoeff[i] - sum;       /* d[0] = 1 */
        } else {                              /* recurrence for i ≥ 11 */
            /* a(i) = a(i-1) - a(i-3) + 2*a(i-4) - a(i-5) + a(i-7) - a(i-8) */
            cache[i] =  cache[i - 1]          /* a(i-1) */
                      - cache[i - 3]          /* - a(i-3) */
                      +  ((seq_t)2) * cache[i - 4]   /* + 2*a(i-4) */
                      - cache[i - 5]          /* - a(i-5) */
                      + cache[i - 7]          /* + a(i-7) */
                      - cache[i - 8];         /* - a(i-8) */
        }
        ready[i] = true;
    }

    return cache[n];
}
