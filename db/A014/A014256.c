/*
 * A014256  Inverse of 247th cyclotomic polynomial.
 * https://oeis.org/A014256
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

/* Sequence A014256: coefficients of the formal power series 1/Φ₍₂₄₇₎(x).
   Let Φ₍₂₄₇₎(x) = 1 + φ₁x + φ₂x² + … + φ₂₁₆x²¹⁶.
   Then a(0) = 1 and for n ≥ 1
       a(n) = – Σ_{i=1}^{min(n,216)} φᵢ · a(n–i).
   The coefficients φᵢ are all in {–1,0,1} and are computed once by long division. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ----------  coefficients of Φ₍₂₄₇₎(x)  ---------- */
static int8_t phi_coef[217];      /* φ₀ … φ₂₁₆  (φ₀ is 1) */
static bool    phi_inited = false;

/* compute φᵢ by dividing the numerator N(x)=x²⁴⁸–x²⁴⁷–x+1
   by the denominator D(x)=x³²–x¹⁹–x¹³+1  */
static void init_phi(void)
{
    if (phi_inited) return;
    phi_inited = true;

    /* constants */
    const int degN = 248;          /* highest exponent in N(x) */
    const int degD = 32;           /* highest exponent in D(x) */
    const int degQ = degN - degD;  /* degree of Φ₍₂₄₇₎(x) = 216 */

    /* working copies of the polynomials, zeroed */
    int A[degN + 1];
    int D[degD + 1];
    memset(A, 0, sizeof A);
    memset(D, 0, sizeof D);

    /* N(x) = x²⁴⁸ – x²⁴⁷ – x + 1 */
    A[0]   =  1;   /* +1 */
    A[1]   = -1;   /* –x */
    A[247] = -1;   /* –x²⁴⁷ */
    A[248] =  1;   /* +x²⁴⁸ */

    /* D(x) = x³² – x¹⁹ – x¹³ + 1 */
    D[0]   =  1;   /* +1 */
    D[13]  = -1;   /* –x¹³ */
    D[19]  = -1;   /* –x¹⁹ */
    D[32]  =  1;   /* +x³² */

    /* long division: A(x) ÷ D(x)  ->  Q(x) = Φ₍₂₄₇₎(x) */
    for (int i = degQ; i >= 0; --i)
    {
        int q = A[i + degD];              /* leading coefficient of the current remainder */
        phi_coef[i] = (int8_t)q;          /* coefficient of Φ₍₂₄₇₎(x) */
        if (q != 0)
        {
            /* subtract q * D(x) shifted by i */
            for (int j = 0; j <= degD; ++j)
                A[i + j] -= q * D[j];
        }
    }
}

/* ----------  public function  ---------- */
seq_t a(idx_t n)
{
    init_phi();                       /* ensure φᵢ are ready */

    /* the sequence is trivial for n = 0 */
    if (n == 0) return 1;

    /* allocate array for a[0…n] */
    seq_t *vals = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!vals) return 0;              /* out of memory – terminate gracefully */

    vals[0] = 1;                      /* a(0) */

    /* compute terms up to n */
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sum = 0;
        idx_t limit = (k < 216 ? k : 216);  /* i runs 1 … min(k,216) */
        for (idx_t i = 1; i <= limit; ++i)
            sum += (seq_t)phi_coef[i] * vals[k - i];
        vals[k] = -sum;
    }

    seq_t result = vals[n];
    free(vals);
    return result;
}
