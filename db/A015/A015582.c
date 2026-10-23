/*
 * A015582  Inverse of 1573rd cyclotomic polynomial.
 * https://oeis.org/A015582
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

/*
 * Formula: a(n) is the coefficient of x^n in the formal power series
 * expansion of 1/Φ₁₅₇₃(x) (the inverse of the 1573‑th cyclotomic
 * polynomial).  The sequence is periodic with period 1573.
 */

#include <stdint.h>
#include <stdbool.h>

/* forward declaration of the function */
seq_t a(idx_t n);

/* static data used by the routine */
static bool initialized = false;
static seq_t C[1573];   /* coefficients of Φ₁₅₇₃(x) modulo x^1573 */
static seq_t S[1573];   /* coefficients of its reciprocal */

/* ------------------------------------------------------------------ */
/* initialise the static tables once */
static void init_tables(void)
{
    if (initialized)
        return;

    /* degrees that appear in Φ₁₅₇₃(x) via the identity
     * Φ₁₅₇₃(x) = (x^1573 - 1)(x^11 - 1) / ((x^143 - 1)(x^121 - 1))
     * Numerator degree = 1584, denominator degree = 264.
     */
    const int degN = 1584;
    const int degD = 264;
    seq_t N[1585] = {0};
    seq_t D[265]  = {0};
    seq_t Q[1585] = {0};

    /* build the numerator: x^1584 - x^1573 - x^11 + 1  */
    N[1584] =  1;
    N[1573] = -1;
    N[11]   = -1;
    N[0]    =  1;

    /* build the denominator: x^264 - x^143 - x^121 + 1   */
    D[264] =  1;
    D[143] = -1;
    D[121] = -1;
    D[0]   =  1;

    /* integer polynomial division N / D  */
    for (int i = degN; i >= degD; --i) {
        seq_t coeff = N[i];
        if (coeff != 0) {
            int shift = i - degD;
            Q[shift] = coeff;
            for (int j = 0; j <= degD; ++j) {
                /* N[j+shift] -= coeff * D[j]  */
                N[j + shift] -= coeff * D[j];
            }
        }
    }

    /* store Φ₁₅₇₃(x) modulo x^1573: coefficients up to degree 1320 */
    for (int i = 0; i < 1573; ++i) {
        if (i <= 1320)
            C[i] = Q[i];
        else
            C[i] = 0;
    }

    /* compute the reciprocal series S via the recurrence
     *   S[0] = 1
     *   S[n] = -∑_{k=1..n} C[k] * S[n-k]   (for n ≥ 1)
     * This yields S(x) ≡ 1/Φ₁₅₇₃(x) (mod x^1573)
     */
    S[0] = 1;
    for (int n = 1; n < 1573; ++n) {
        seq_t sum = 0;
        for (int k = 1; k <= n; ++k) {
            /* term is zero for most k, but we multiply anyway */
            sum += C[k] * S[n - k];
        }
        S[n] = -sum;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* public function required by the exercise                         */
seq_t a(idx_t n)
{
    init_tables();
    if (n >= 1573)
        n %= 1573;                 /* use the period */
    return S[n];
}
