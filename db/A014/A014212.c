/*
 * A014212  Inverse of 203rd cyclotomic polynomial.
 * https://oeis.org/A014212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A014212: Inverse of the 203rd cyclotomic polynomial.
   The sequence satisfies the linear recurrence
       a(0) = 1,
       a(n) = -∑_{i=1}^{min(n,168)} c_i · a(n-i)   (n>0)
   where the c_i are the coefficients of Φ_203(x).  The sequence is
   periodic with period 203. */

#include <stdbool.h>

static seq_t period[203] = {0};
static bool initialized = false;

static void init_seq(void) {
    if (initialized)
        return;

    /* ----------------- 1.  Compute Φ_203(x) coefficients ----------------- */
    /*  Φ_203(x) = (x^203 - 1) * (x - 1) / ((x^29 - 1) * (x^7 - 1)) */
    /*  Numerator:  x^204 - x^203 - x + 1  */
    seq_t num[205] = {0};
    num[204] = 1;
    num[203] = -1;
    num[1]   = -1;
    num[0]   = 1;

    /*  Denominator:  x^36 - x^29 - x^7 + 1  */
    seq_t den[37] = {0};
    den[36] = 1;
    den[29] = -1;
    den[7]  = -1;
    den[0]  = 1;

    /*  Long division:  num / den  */
    seq_t coef[169] = {0};               /* coefficients of Φ_203, degrees 0..168 */
    for (idx_t i = 204; i >= 36; --i) {
        seq_t factor = num[i];
        idx_t idx   = i - 36;            /* position in result polynomial */
        coef[idx]   = factor;
        /* subtract factor * den * x^idx from num */
        for (idx_t j = 0; j <= 36; ++j) {
            num[idx + j] -= factor * den[j];
        }
    }
    /*  (Optional sanity check: remainder should be zero) */

    /* ----------------- 2.  Compute period array (first 203 terms) --------- */
    period[0] = 1;
    for (idx_t n = 1; n < 203; ++n) {
        seq_t sum = 0;
        idx_t max_i = (n <= 168) ? n : 168;
        for (idx_t i = 1; i <= max_i; ++i) {
            sum += coef[i] * period[n - i];
        }
        period[n] = -sum;
    }

    initialized = true;
}

/* Public interface required by the problem statement. */
seq_t a(idx_t n) {
    init_seq();                      /* ensure period is prepared once */
    idx_t idx = n % 203;             /* period 203 */
    return period[idx];
}
