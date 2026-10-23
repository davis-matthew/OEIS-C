/*
 * A015688  Inverse of 1679th cyclotomic polynomial.
 * https://oeis.org/A015688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; for n > 0,
 *   a(n) = - Σ_{k=1}^{min(n,φ(1679))} a(n-k) * c_k ,
 * where c_k are the coefficients of the 1679‑th cyclotomic polynomial Φ₁₆₇₉(x).
 * The sequence is periodic with period 1679.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static seq_t *coeff = NULL;       /* coefficients of Φ₁₆₇₉(x)  (size 1585) */
static seq_t *a_table = NULL;     /* period 1679 values of the sequence */
static bool init_done = false;

static void compute_coeffs(void)
{
    /*  Φ_{1679} = (x^{1679}-1) * (x-1) / ((x^{73}-1)(x^{23}-1))  */
    const size_t degN = 1680;    /* degree of numerator */
    const size_t degD = 96;      /* degree of denominator  */
    const size_t degQ = degN - degD;  /* degree of quotient Φ₁₆₇₉ */
    const size_t period = 1679;         

    /* mutable copy of numerator coefficients in 64‑bit ints */
    long long *num = (long long *) calloc(degN + 1, sizeof(long long));
    long long *den = (long long *) calloc(degD + 1, sizeof(long long));

    /* numerator: x^{1680} - x^{1679} - x + 1 */
    num[0]  = 1;
    num[1]  = -1;
    num[1679] = -1;
    num[1680] = 1;

    /* denominator: x^{96} - x^{73} - x^{23} + 1 */
    den[0]  = 1;
    den[23] = -1;
    den[73] = -1;
    den[96] = 1;

    /* quotient coefficients (as long long) */
    long long *quot = (long long *) calloc(degQ + 1, sizeof(long long));

    for (int i = (int)degN; i >= (int)degD; --i) {
        long long q = num[i];
        if (q == 0) continue;
        quot[i - (int)degD] = q;
        for (size_t j = 0; j <= degD; ++j) {
            num[i - degD + j] -= q * den[j];
        }
    }

    /* store coefficients as seq_t */
    coeff = (seq_t *) malloc((degQ + 1) * sizeof(seq_t));
    for (size_t i = 0; i <= degQ; ++i) {
        coeff[i] = (seq_t) quot[i];
    }

    free(num);
    free(den);
    free(quot);
}

static void compute_sequence(void)
{
    const size_t period = 1679;
    const size_t degC = 1584;

    a_table = (seq_t *) malloc(period * sizeof(seq_t));
    a_table[0] = (seq_t)1;

    for (size_t n = 1; n < period; ++n) {
        seq_t sum = 0;
        size_t limit = (n <= degC) ? n : degC;
        for (size_t k = 1; k <= limit; ++k) {
            sum += a_table[n - k] * coeff[k];
        }
        a_table[n] = -sum;
    }
}

static void init_sequence(void)
{
    if (!init_done) {
        compute_coeffs();
        compute_sequence();
        init_done = true;
    }
}

seq_t a(idx_t n)
{
    init_sequence();
    const size_t period = 1679;
    return a_table[n % period];
}
