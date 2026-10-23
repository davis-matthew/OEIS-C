/*
 * A015600  Inverse of 1591st cyclotomic polynomial.
 * https://oeis.org/A015600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = the n-th coefficient of the power series expansion of
 *            1/Φ₁₅₉₁(x). The sequence is periodic of period 1591
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------
   Internal data for the coefficients of Φ1591(x) and its inverse.
   ------------------------------------------------------------- */

static const idx_t PERIOD = 1591;           /* period length                */
static const idx_t DEG_PHI = 1512;          /* degree of Φ1591(x)           */

static seq_t *phi_coef = NULL;             /* coefficients of Φ1591(x)     */
static seq_t *inv_coef = NULL;             /* coefficients of 1/Φ1591(x)   */
static bool initialized = false;

/* -------------------------------------------------------------
   Compute the coefficients of Φ1591(x) using the identity
        Φ_n(x) = (x^n - 1) * (x - 1) / [(x^{n/37} - 1)*(x^{n/43} - 1)]
   for n = 1591 = 37·43.
   ------------------------------------------------------------- */
static void init_phi_coeffs(void)
{
    /* Allocate storage for the coefficients of Φ1591(x)                 */
    phi_coef = (seq_t*)malloc((DEG_PHI + 1) * sizeof(seq_t));
    if (!phi_coef) return;                                 /* OOM guard */

    /* D(x) = (x^43 - 1)*(x^37 - 1) = x^80 - x^43 - x^37 + 1 */
    const idx_t d0 = 0, d37 = 37, d43 = 43, d80 = 80;
    const seq_t D37 = (seq_t)(-1);   /* coefficient of x^37   */
    const seq_t D43 = (seq_t)(-1);   /* coefficient of x^43   */
    const seq_t D80 = (seq_t)( 1);   /* coefficient of x^80   */

    /* The numerator N(x) = (x^1591 - 1)*(x - 1) = x^1592 - x^1591 - x + 1 */
    for (idx_t i = 0; i <= DEG_PHI; ++i) {      /* φ coefficients ∈ [0..1512] */
        seq_t sum = 0;

        if (i >= d37) sum += D37 * phi_coef[i - d37];
        if (i >= d43) sum += D43 * phi_coef[i - d43];
        if (i >= d80) sum += D80 * phi_coef[i - d80];

        seq_t Ni = 0;
        if (i == 0) Ni = (seq_t)1;
        else if (i == 1) Ni = (seq_t)-1;   /* N₀ = 1, N₁ = -1; others 0 for i ≤ 1512 */

        phi_coef[i] = Ni - sum;              /* D₀ = 1, so divide by 1 */
    }
}

/* -------------------------------------------------------------
   Compute the coefficients of 1/Φ1591(x) up to the period length
   and store them in inv_coef[0 .. PERIOD-1].  The series
   satisfies Φ(x)·S(x) = 1, i.e.  a₀ = 1 and for n > 0
   aₙ = -∑_{k=1}^{min(n,degΦ)} φ_k · a_{n-k}.
   ------------------------------------------------------------- */
static void init_inv_coeffs(void)
{
    /* Allocate array for the inverse coefficients */
    inv_coef = (seq_t*)malloc(PERIOD * sizeof(seq_t));
    if (!inv_coef) return;                                 /* OOM guard */

    inv_coef[0] = (seq_t)1;   /* a₀ = 1 */

    for (idx_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        idx_t maxk = (n < (idx_t)DEG_PHI) ? n : (idx_t)DEG_PHI;

        for (idx_t k = 1; k <= maxk; ++k) {
            sum += phi_coef[k] * inv_coef[n - k];
        }
        inv_coef[n] = -sum;
    }
}

/* -------------------------------------------------------------
   Main sequence function.  On the first call it initializes
   the lookup tables; afterwards it simply returns the cached
   value indexed modulo the period.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_phi_coeffs();
        init_inv_coeffs();
        initialized = true;
    }
    idx_t r = n % PERIOD;
    return inv_coef[r];
}
