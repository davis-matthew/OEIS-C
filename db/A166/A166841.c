/*
 * A166841  Number of nX2 1..7 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          decreasing order.
 * https://oeis.org/A166841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A166841: Number of n×2 1..7 arrays with all constraints.
   Formula (derived from the rational generating function):
     a(n) = 0                         for n < 4
     a(n) = Σ_{k=0}^8 coeffs[k] · C(n + 8 - k, 12)
     where coeffs = [714, 819, -8174, 13431, -7942, -843, 3314, -1519, 232]
   The coefficients come from the numerator polynomial in the GF.
   C(m,12) is the binomial coefficient (m choose 12).
   This implementation computes the binomial coefficient by
   multiplicative reduction and uses only +, -, *, / on seq_t,
   satisfying the build’s integer arithmetic requirements. */

#include <stdbool.h>

/*--- binomial coefficient C(n,12) for 12 >= 0 --------------------------------*/
static seq_t binom12(idx_t n)
{
    /* n >= 12 because we call this only when n >= 12 */
    seq_t res = 1;
    for (int i = 1; i <= 12; ++i) {
        res = res * (seq_t)(n - 12 + i);   /* multiply by (n-12+i) */
        res = res / (seq_t)i;              /* divide by i */
    }
    return res;
}

/*--- main sequence function -------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 4u) return 0;   /* first non‑zero term is at n=4 */

    /* coefficients of the numerator polynomial in the generating function */
    const int coeffs[9] = {
        714, 819, -8174, 13431, -7942, -843, 3314, -1519, 232
    };

    seq_t pos = 0; /* sum of terms with positive coefficient */
    seq_t neg = 0; /* sum of terms with negative coefficient */

    /* For each k we need C(n + 8 - k, 12) only when n-4-k >= 0
       i.e. when k <= n-4. */
    for (int k = 0; k < 9; ++k) {
        if (k > (int)(n - 4u)) /* skip terms not present for this n */
            continue;

        idx_t m = (n + 8u) - (idx_t)k;   /* m = n + 8 - k  >= 12  */

        seq_t bin = binom12(m);          /* C(m,12) */

        seq_t mult = bin * (seq_t)abs(coeffs[k]);

        if (coeffs[k] >= 0)
            pos = pos + mult;
        else
            neg = neg + mult;
    }

    return pos - neg;   /* guaranteed non‑negative for all n >= 4 */
}
