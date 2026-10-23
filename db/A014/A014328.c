/*
 * A014328  Inverse of 319th cyclotomic polynomial.
 * https://oeis.org/A014328
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

/* Sequence A014328: coefficients of the formal power series 1/Φ_319(x).  
   The sequence is periodic with period 319.  
   The coefficients Φ_k of the 319‑th cyclotomic polynomial are computed
   by long division, after which the recurrence  
   a(n) = -Σ_{k=1}^{min(n,280)} Φ_k · a(n-k) (with a(0)=1) yields the
   first 319 terms, which then repeat. */

#include <stdint.h>

#define P          319          /* period length */
#define DEG_NUM    320          /* degree of numerator (x-1)(x^319-1) */
#define DEG_DEN    40           /* degree of denominator (x^11-1)(x^29-1) */
#define DEG_Q      (DEG_NUM - DEG_DEN)  /* degree of Φ_319(x) */

static void compute_phi_coeffs(int64_t *q)
/* Fill q[0..DEG_Q] with the coefficients of Φ_319(x) */
{
    int64_t num[DEG_NUM + 1] = {0};
    num[DEG_NUM] = 1;        /* x^320 */
    num[DEG_NUM - 1] = -1;   /* -x^319 */
    num[1] = -1;            /* -x */
    num[0] = 1;             /* +1 */

    int64_t den[DEG_DEN + 1] = {0};
    den[DEG_DEN] = 1;        /* x^40 */
    den[29] = -1;            /* -x^29 */
    den[11] = -1;            /* -x^11 */
    den[0] = 1;             /* +1 */

    for (int i = DEG_NUM; i >= DEG_DEN; --i) {
        int64_t coeff = num[i];
        int shift = i - DEG_DEN;
        q[shift] = coeff;
        if (coeff != 0) {
            for (int j = 0; j <= DEG_DEN; ++j) {
                num[shift + j] -= coeff * den[j];
            }
        }
    }
}

static void init_period(seq_t *period)
/* Compute the first period (P) coefficients of 1/Φ_319(x) */
{
    int64_t q_coeff[DEG_Q + 1];
    compute_phi_coeffs(q_coeff);   /* q_coeff[0] = 1, q_coeff[1..DEG_Q] are Φ_k */

    period[0] = (seq_t)1;
    for (int n = 1; n < P; ++n) {
        int64_t sum = 0;
        int top = (n < DEG_Q) ? n : DEG_Q;
        for (int k = 1; k <= top; ++k) {
            sum += q_coeff[k] * (int64_t)period[n - k];
        }
        period[n] = (seq_t)(-sum);
    }
}

seq_t a(idx_t n)
{
    static seq_t period[P];
    static int initialized = 0;
    if (!initialized) {
        init_period(period);
        initialized = 1;
    }
    return period[n % P];
}
