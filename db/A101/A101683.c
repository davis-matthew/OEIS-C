/*
 * A101683  Write exp(sqrt(1+x)-1) = Sum c(n) x^n/n!; then a(n) = numerator of
 *          c(n).
 * https://oeis.org/A101683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) is the numerator of the reduced fraction c(n) given by c(0)
 *            = 1, c(1) = 1/2, c(n) = (c(n-2) - (4*n-6)*c(n-1))/4 (n ≥ 2)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 29,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 403 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

/* helper: left‑shift a value by shift bits.
   For small shift (≤63) use the builtin operator; otherwise
   multiply by 2 repeatedly.  Works for both builtin and
   arbitrary‑precision seq_t types. */
static seq_t lshift(seq_t val, idx_t shift)
{
    if (shift == 0) return val;
    /* small shift uses builtin operation (portable for all types) */
    if (shift <= 63) {
        return val << (int)shift;
    }
    /* large shift – multiply by 2 in a loop (safe for arbitrary‑precision) */
    for (idx_t i = 0; i < shift; ++i) {
        val <<= 1;
    }
    return val;
}

/* numerators of the reduced c(n) (odd or 0) */
static seq_t *numer = NULL;
/* exponent of 2 in the reduced denominator of c(n) */
static idx_t *expv  = NULL;
/* largest n that has been computed */
static idx_t computed_max = 0;

/* initialise base cases for n = 0,1 */
static void ensure_base(void)
{
    if (computed_max < 1) {
        idx_t sz = 2;
        numer = seq_malloc(sz);
        expv  = (idx_t*)malloc(sz * sizeof(idx_t));

        numer[0] = (seq_t)1;  expv[0] = (idx_t)0;   /* c(0) = 1/1   */
        numer[1] = (seq_t)1;  expv[1] = (idx_t)1;   /* c(1) = 1/2   */

        computed_max = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        ensure_base();
        return numer[n];
    }

    ensure_base();

    /* grow storage if necessary */
    idx_t newsize = n + 1;
    if (numer == NULL) {
        numer = seq_malloc(newsize);
        expv  = (idx_t*)malloc(newsize * sizeof(idx_t));
    } else if (newsize > computed_max + 1) {
        numer = seq_realloc(numer, newsize);
        expv  = (idx_t*)realloc(expv, newsize * sizeof(idx_t));
    }

    /* compute all missing terms */
    for (idx_t k = computed_max + 1; k <= n; ++k) {
        seq_t a_prev2 = numer[k-2];
        seq_t a_prev1 = numer[k-1];
        idx_t e_prev2 = expv[k-2];
        idx_t e_prev1 = expv[k-1];

        idx_t s = e_prev2 > e_prev1 ? e_prev2 : e_prev1;
        idx_t shift1 = s - e_prev2;   /* ≥0 */
        idx_t shift2 = s - e_prev1;   /* ≥0 */

        /* term1 = a_prev2 * 2^{shift1} */
        seq_t term1 = lshift(a_prev2, shift1);
        /* term2 = (4*k-6) * a_prev1 * 2^{shift2} */
        seq_t term2 = lshift(a_prev1 * (seq_t)(4*k - 6), shift2);

        /* numerator before dividing by 2^{s+2} */
        seq_t N = term1 - term2;

        if (N == 0) {
            numer[k] = (seq_t)0;
            expv[k]  = (idx_t)0;
            continue;
        }

        /* extract all factors of 2 from N */
        idx_t v = 0;
        while ((N & (seq_t)1) == 0) {
            N >>= 1;
            ++v;
        }

        numer[k] = N;                          /* odd part */
        expv[k]  = s + 2 - v;                  /* remaining exponent of 2 */
    }

    computed_max = n;
    return numer[n];
}
