/*
 * A157748  Expansion of 1/(-x^11 + x^10 + 2*x^7 - x^6 - x^5 - x^4 - x^2 + 2*x
 *          + 1).
 * https://oeis.org/A157748
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 24, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A157748: Expansion of 1/(-x^11 + x^10 + 2*x^7 - x^6 - x^5 - x^4 - x^2 + 2*x + 1)
   Recurrence: a(0) = 1;
   For n ≥ 1:
       a(n) = - ( 2*a(n-1) - a(n-2) - a(n-4) - a(n-5) - a(n-6)
                  + 2*a(n-7) + a(n-10) - a(n-11) )
   (obtained from P(x)*A(x)=1 where
    P(x) = 1 + 2x – x² – x⁴ – x⁵ – x⁶ + 2x⁷ + x¹⁰ – x¹¹) */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[i] holds a(i) */
static idx_t computed = 0;    /* number of terms currently cached (≥1 when cache[0] is set) */

/* Coefficients of P(x) = 1 + 2x – x² – x⁴ – x⁵ – x⁶ + 2x⁷ + x¹⁰ – x¹¹ */
static const int coeff[12] =
    { 1,  2, -1,  0, -1, -1, -1,  2,  0,  0,  1, -1 };

/* Ensure cache contains all terms up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (computed > n)
        return;                       /* already have the required terms */

    idx_t new_cap = n + 1;              /* number of elements needed */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                           /* allocation failed – leave cache unchanged */
        return;

    cache = tmp;

    /* Initialize a(0) if this is the first call */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* Compute the missing terms */
    for (idx_t i = computed; i < new_cap; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 11; ++k) {
            if (i >= (idx_t)k) {
                sum += (seq_t)coeff[k] * cache[i - k];
            }
        }
        cache[i] = -sum;          /* a(n) = -Σ coeff[k]*a(n-k) */
    }

    computed = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
