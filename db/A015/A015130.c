/*
 * A015130  Inverse of 1121st cyclotomic polynomial.
 * https://oeis.org/A015130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; for n ≥ 1,
   a(n) = - ∑_{i=1}^{k} φ_i · a(n–i)
   where the φ_i are the coefficients of the 1121‑st cyclotomic
   polynomial Φ₁₁₂₁(x).  The sequence has period 1121. */

#include <stdbool.h>
#include <stddef.h>

/* φ_i: coefficients of Φ₁₁₂₁(x) (0 ≤ i ≤ 1044) */
static seq_t phi[1045];
/* period array: a(0)…a(1120) */
static seq_t period[1121];
static bool period_computed = false;

/* Compute the coefficients of Φ₁₁₂₁(x) by exact long division. */
static void precompute_phi(void)
{
    /* Φ₁₁₂₁(x) = (x⁻¹–1)(x¹¹²¹–1) / ((x¹⁹–1)(x⁵⁹–1))
       = (x¹¹²₂ – x¹¹²₁ – x + 1) / (x⁷⁸ – x⁵⁹ – x¹⁹ + 1)   */
    seq_t A[1123] = {0};           /* numerator, degree 1122 */
    A[0] = 1;
    A[1] = -1;
    A[1121] = -1;
    A[1122] = 1;

    seq_t B[79] = {0};            /* denominator, degree 78 */
    B[0] = 1;
    B[19] = -1;
    B[59] = -1;
    B[78] = 1;

    /* long division: A / B */
    seq_t rem[1123];
    seq_t q[1045] = {0};          /* quotient, degree 1044 */

    /* copy numerator to remainder */
    for (int i = 0; i <= 1122; ++i)
        rem[i] = A[i];

    /* perform division; B is monic */
    for (int k = 1122; k >= 78; --k) {
        seq_t coeff = rem[k];
        if (coeff != 0) {
            int pos = k - 78;     /* position in q */
            q[pos] = coeff;
            /* subtract coeff * B(x) shifted by pos from remainder */
            for (int i = 0; i <= 78; ++i)
                rem[pos + i] = rem[pos + i] - coeff * B[i];
        }
    }

    /* store quotient coefficients */
    for (int i = 0; i <= 1044; ++i)
        phi[i] = q[i];
}

/* Precompute the period 0…1120 of the sequence. */
static void precompute_period(void)
{
    if (period_computed)
        return;
    period_computed = true;

    precompute_phi();

    period[0] = 1;
    for (int n = 1; n < 1121; ++n) {
        seq_t sum = 0;
        int max_i = (n < 1044) ? n : 1044;
        for (int i = 1; i <= max_i; ++i)
            sum = sum + phi[i] * period[n - i];
        period[n] = -sum;   /* recurrence */
    }
}

/* Return a(n) for arbitrary n (n ≥ 0). */
seq_t a(idx_t n)
{
    precompute_period();
    idx_t idx = n % 1121;
    return period[(size_t)idx];
}
