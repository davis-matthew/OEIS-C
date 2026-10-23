/*
 * A014698  Inverse of 689th cyclotomic polynomial.
 * https://oeis.org/A014698
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

#include <stdbool.h>

/* A014698: a(n) – coefficient of x^n in 1/Φ_689(x)  (period 689) */
static seq_t phi[625];
static seq_t a_coeff[689];
static bool ready = false;

/* compute Φ_689(x) as (x^689 - 1)(x - 1) / ((x^13 - 1)(x^53 - 1)) */
static void compute_sequence(void)
{
    /* ---------- build (x^689 - 1)(x - 1) ----------
       result:  P(x) = x^690 - x^689 - x + 1          */
    seq_t P[691];
    for (int i = 0; i < 691; ++i) P[i] = 0;
    P[0]   =  1;   /* +1   */
    P[1]   = -1;   /* -x   */
    P[689] = -1;   /* -x^689 */
    P[690] =  1;   /* +x^690 */

    /* ---------- divide P by (x^13 - 1) ----------
       divisor: D1(x) = x^13 - 1  (coeff[0] = -1, coeff[13] = 1)  */
    seq_t Q[678];
    seq_t tmp[691];  /* temporary buffer for division */
    for (int i = 0; i < 691; ++i) tmp[i] = P[i];

    for (int i = 690; i >= 13; --i) {
        seq_t q = tmp[i];                /* leading coefficient is 1 */
        int shift = i - 13;
        Q[shift] = q;
        if (q != 0) {
            tmp[shift]     += q;         /* coeff[0] = -1 → subtract (-q) */
            tmp[shift + 13] -= q;        /* coeff[13] = 1  → subtract q   */
            /* other coefficients of D1 are zero, skip */
        }
    }
    /* ---------- divide Q by (x^53 - 1) ----------
       divisor: D2(x) = x^53 - 1  (coeff[0] = -1, coeff[53] = 1) */
    seq_t R[678];
    for (int i = 0; i < 678; ++i) R[i] = Q[i];  /* copy for second division */

    for (int i = 677; i >= 53; --i) {
        seq_t q = R[i];
        int shift = i - 53;
        phi[shift] = q;
        if (q != 0) {
            R[shift]     += q;          /* coeff[0] = -1  → subtract (-q) */
            R[shift + 53] -= q;         /* coeff[53] = 1  → subtract q  */
            /* other coefficients are zero */
        }
    }
    /* phi[0] should now be 1; no check necessary – it follows from the
       construction of Φ_689.  */

    /* ---------- compute a(n) for n = 0..688 ----------
       recurrence: a(0) = 1
       for n >= 1: a(n) = - Σ_{k=1}^{min(n,624)} φ(k) * a(n-k)   */
    a_coeff[0] = 1;
    for (int n = 1; n < 689; ++n) {
        seq_t sum = 0;
        int maxk = (n <= 624) ? n : 624;
        for (int k = 1; k <= maxk; ++k) {
            sum += phi[k] * a_coeff[n - k];
        }
        a_coeff[n] = -sum;
    }
}

seq_t a(idx_t n)
{
    if (!ready) {
        compute_sequence();
        ready = true;
    }
    return a_coeff[n % 689];
}
