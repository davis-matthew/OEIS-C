/*
 * A058636  McKay-Thompson series of class 33A for Monster.
 * https://oeis.org/A058636
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 46, i16 n <= 139, i32 n <= 454, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: A058636 is the McKay-Thompson series of class 33A for the Monster.  
 * a(n) is the coefficient of q^n in
 *
 *   1 + η(q)·η(q^11) / (η(q^3)·η(q^33))
 *
 * where the Dedekind eta function is η(q)=q^(1/24)∏_{k≥1}(1−q^k).
 * The ratio has the form q^(−1)·S(q) with
 *
 *   S(q) = ∏_{k≥1} (1−q^k)(1−q^{11k}) / (1−q^{3k})(1−q^{33k}).
 *
 * The expansion of S(q) can be obtained by multiplying the factors
 * (1−q^t) and the inverses (1−q^t)^{-1} in the appropriate order.
 *
 * The implementation builds the sequence S up to the required index
 * in O(N^2) time (≈70 M elementary operations for N=10 000) and
 * stores it in a static cache for repeated calls.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *S = NULL;        /* cached coefficients of S(q)          */
static size_t cap = 0;        /* current capacity of S                 */

/* Build S[0..limit] where limit >= 0.  All indices are zero‑based. */
static void build_series(size_t limit)
{
    seq_t *newS = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!newS) return; /* allocation failed; caller will handle */

    newS[0] = 1;
    for (size_t i = 1; i <= limit; ++i) newS[i] = 0;

    for (size_t k = 1; k <= limit; ++k) {
        /* factor (1 - q^k) */
        size_t t1 = k;
        for (size_t i = limit; i >= t1; --i) newS[i] -= newS[i - t1];

        /* factor (1 - q^{11k}) if it fits */
        size_t t2 = 11 * k;
        if (t2 <= limit) {
            for (size_t i = limit; i >= t2; --i) newS[i] -= newS[i - t2];
        }

        /* factor (1 - q^{3k})^{-1} */
        size_t t3 = 3 * k;
        if (t3 <= limit) {
            for (size_t i = t3; i <= limit; ++i) newS[i] += newS[i - t3];
        }

        /* factor (1 - q^{33k})^{-1} */
        size_t t4 = 33 * k;
        if (t4 <= limit) {
            for (size_t i = t4; i <= limit; ++i) newS[i] += newS[i - t4];
        }
    }

    free(S);
    S = newS;
    cap = limit + 1;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ –1. */
    if (n == -1) return (seq_t)1;         /* coefficient of q^(-1) */
    if (n < -1) return (seq_t)0;          /* undefined behaviour    */

    /* We need S up to index n+1 (since R(q)=q^-1·S(q)). */
    size_t need = (size_t)(n + 2);         /* >=1 for n>=-1 */
    if (need > cap) build_series(need - 1);

    if (n == 0)                          /* a(0) = 1 + coefficient of q^0 in R */
        return (seq_t)1 + S[1];

    return S[n + 1];                     /* a(n) = coefficient of q^n in R for n≥1 */
}
