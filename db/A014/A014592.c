/*
 * A014592  Inverse of 583rd cyclotomic polynomial.
 * https://oeis.org/A014592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ₅₈₃(x), where Φ₅₈₃(x) =
 *            Φ₁₁(x³⁰⁹)/Φ₁₁(x). The sequence is periodic with period 583
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 583 = 11 · 53  (both primes).  For n = p·q with p<q,
    Φₙ(x) = Φ_p(x^q) / Φ_p(x).  Here Φ₁₁(x) = 1 + x + … + x¹⁰.
    Consequently Φ₅₈₃ has degree φ(583)=10·52=520.  */

#include <stdbool.h>
#include <stddef.h>

static seq_t c_coeff[521];           /* Φ₅₈₃(x) coefficients, degree 520 */
static seq_t inv_series[583];        /* first 583 coefficients of 1/Φ₅₈₃(x) */
static bool initialized = false;     /* one‑time initialization flag */

seq_t a(idx_t n)
{
    if (!initialized)
    {
        /* ---------- 1. Build Φ₅₈₃(x) via exact division ----------
           Let A(x) = 1 + x + … + x¹⁰.
           Let B(x) = 1 + x⁵³ + x¹⁰⁶ + … + x⁵³⋅₁₀ = 1 + x⁵³ + … + x⁵³⁰.
           Compute Q(x) = B(x)/A(x)  (exactly).  Q has degree 520.
        */
        seq_t R[531] = {0};   /* working remainder, indices 0 … 530 */

        /* Load B(x) into R: coefficients 1 at multiples of 53 */
        for (idx_t i = 0; i <= 10; ++i)
            R[53 * i] = 1;

        /* Long division: successively subtract A(x) * coeff from R. */
        for (idx_t k = 0; k <= 520; ++k)
        {
            /* Leading term of current remainder becomes coeff of Q */
            c_coeff[k] = R[k];

            /* Subtract coeff * A(x) from R starting at exponent k */
            for (idx_t j = 0; j <= 10; ++j)
                R[k + j] -= c_coeff[k];
        }

        /* ---------- 2. Compute inverse series 1/Φ₅₈₃(x) ----------
           Let c[0] = 1; for n >= 1:
           inv[n] = - Σ_{k=1}^{min(n,520)} c[k] * inv[n-k]
        */
        inv_series[0] = 1;
        for (idx_t n = 1; n < 583; ++n)
        {
            seq_t sum = 0;
            idx_t limit = (n <= 520) ? n : 520;
            for (idx_t k = 1; k <= limit; ++k)
                sum += c_coeff[k] * inv_series[n - k];
            inv_series[n] = -sum;
        }

        initialized = true;
    }

    /* Return the periodic value.  Casting to size_t is safe because
       583 < SIZE_MAX. */
    return inv_series[(size_t)(n % 583)];
}
