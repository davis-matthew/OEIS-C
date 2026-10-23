/*
 * A015660  Inverse of 1651st cyclotomic polynomial.
 * https://oeis.org/A015660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) is the coefficient of x^n in 1 / Φ₁₆₅₁(x) (the 1651st
 *            cyclotomic polynomial). The sequence is periodic with period
 *            1651 and satisfies the recurrence a(0)=1, a(n) =
 *            -sum_{k=1}^{min(n,1512)} c_k * a(n-k), where c_k are the
 *            coefficients of Φ₁₆₅₁(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define PERIOD 1651
#define DEG_PH 1512
#define MAX_DEG 1652   /* degree of (x‑1)(x¹⁶⁵¹‑1) */
#define MAX_Q_DEG 140  /* degree of (x¹³‑1)(x¹²⁷‑1) */

static seq_t coeff_phi[DEG_PH + 1];   /* coefficients of Φ₁₆₅₁(x)     */
static seq_t seq_val[PERIOD];         /* one period of the sequence   */

/* Compute the coefficients of the 1651st cyclotomic polynomial
   using polynomial long division. */
static void init_phi(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    seq_t P[MAX_DEG + 1] = {0};
    seq_t Q[MAX_Q_DEG + 1] = {0};
    seq_t R[MAX_DEG + 1];

    /* (x‑1)(x¹⁶⁵¹‑1) = x¹⁶⁵² – x¹⁶⁵¹ – x + 1 */
    P[0]      =  1;
    P[1]      = -1;
    P[1651]   = -1;
    P[1652]   =  1;

    /* (x¹³‑1)(x¹²⁷‑1) = x¹⁴⁰ – x¹³ – x¹²⁷ + 1 */
    Q[0]      =  1;
    Q[13]     = -1;
    Q[127]    = -1;
    Q[140]    =  1;

    /* copy P into the remainder */
    for (int i = 0; i <= MAX_DEG; ++i) R[i] = P[i];

    /* long division:  P / Q  (Q is monic)  →  coeff_phi */
    for (int i = MAX_DEG; i >= MAX_Q_DEG; --i) {
        seq_t factor = R[i];
        int shift = i - MAX_Q_DEG;
        if (factor != 0) {
            for (int j = 0; j <= MAX_Q_DEG; ++j) {
                R[j + shift] -= factor * Q[j];
            }
        }
        coeff_phi[shift] = factor;
    }
}

/* Compute the period of the sequence using the recurrence. */
static void init_sequence(void)
{
    static int done = 0;
    if (done) return;
    done = 1;

    init_phi();

    seq_val[0] = 1;
    for (int i = 1; i < PERIOD; ++i) {
        seq_t sum = 0;
        int maxk = (i <= DEG_PH ? i : DEG_PH);
        for (int k = 1; k <= maxk; ++k) {
            sum += coeff_phi[k] * seq_val[i - k];
        }
        seq_val[i] = -sum;
    }
}

seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_sequence();
        initialized = 1;
    }
    return seq_val[n % PERIOD];
}
