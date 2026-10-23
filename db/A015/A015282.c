/*
 * A015282  Inverse of 1273rd cyclotomic polynomial.
 * https://oeis.org/A015282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the power series expansion of
 *            1/Φ_1273(x) where Φ_1273(x) =
 *            (x^1273-1)(x-1)/((x^19-1)(x^67-1)). The Laurent series satisfies
 *            the linear recurrence a(0)=1; for n>0: a(n) = -∑_{k=1}^{1188}
 *            φ_k · a(n-k), where φ_k are the coefficients of Φ_1273(x). The
 *            sequence is period...
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Forward declaration required by the build system */
seq_t a(idx_t n);

/* Shared tables (initialized lazily) */
static int initialized = 0;
static seq_t *phi_coef = NULL;      /* φ_0 … φ_1188  (size 1189) */
static seq_t *period_terms = NULL;  /* a(0) … a(1272) (size 1273) */

static void init_sequence(void)
{
    if (initialized)
        return;

    /* Allocate tables **/
    phi_coef = (seq_t *)malloc(1189 * sizeof(seq_t));
    period_terms = (seq_t *)malloc(1273 * sizeof(seq_t));

    /* ---------- Build numerator (x^1273-1)(x-1) ---------- */
    seq_t num[1275] = {0};
    num[1274] = (seq_t)1;      /* x^1274   */
    num[1273] = (seq_t)(-1);   /* -x^1273  */
    num[1]    = (seq_t)(-1);   /* -x       */
    num[0]    = (seq_t)1;      /* +1       */

    /* ---------- Build denominator (x^19-1)(x^67-1) ---------- */
    seq_t den[87] = {0};
    den[86] = (seq_t)1;       /* x^86    */
    den[67] = (seq_t)(-1);    /* -x^67   */
    den[19] = (seq_t)(-1);    /* -x^19   */
    den[0]  = (seq_t)1;       /* +1      */

    /* ---------- Long division: num / den ---------- */
    /* num is modified in place, becoming the remainder (which should be 0) */
    for (int i = 1274; i >= 86; --i) {     /* i = current highest degree */
        seq_t q = num[i];                  /* leading coefficient of remainder */
        if (q != 0) {
            int shift = i - 86;
            phi_coef[shift] = q;           /* coefficient of x^shift in quotient */

            /* subtract q * den(x) shifted by 'shift' from num */
            for (int j = 0; j <= 86; ++j) {
                num[shift + j] -= q * den[j];
            }
        }
    }

    /* ---------- Compute first 1273 terms of the series ---------- */
    period_terms[0] = (seq_t)1;
    for (int n = 1; n < 1273; ++n) {
        seq_t sum = (seq_t)0;
        int kmax = (n <= 1188) ? n : 1188;
        for (int k = 1; k <= kmax; ++k) {
            sum += phi_coef[k] * period_terms[n - k];
        }
        period_terms[n] = -sum;
    }

    /* Mark initialization complete */
    initialized = 1;
}

seq_t a(idx_t n)
{
    init_sequence();
    idx_t idx = n % 1273;
    return period_terms[(int)idx];
}
