/*
 * A014952  Inverse of 943rd cyclotomic polynomial.
 * https://oeis.org/A014952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in the formal power series 1 /
 *            Φ_943(x). The sequence is periodic with period 943, so a(n) =
 *            a(n mod 943). We compute the coefficients of Φ_943(x) by
 *            dividing (x^944 - x^943 - x + 1) by (x^64 - x^41 - x^23 + 1).
 *            Then we evaluate the recurrence a(n) = -∑_{k=1}^{min(n,880)...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------- */
/*  Constants for the polynomial sizes                          */
/* ------------------------------------------------------------- */
#define DEG_PHI 880        /* φ(943) = 880   */
#define DEG_R   944        /* degree of R(x) */
#define DEG_D   64         /* degree of D(x) */
#define PERIOD 943

/* ------------------------------------------------------------- */
/*  Storage for the cyclotomic coefficients c_k                */
/* ------------------------------------------------------------- */
static bool coeffs_ready = false;
static seq_t coeffs[DEG_PHI + 1];   /* c_0 .. c_880, c_0 = 1 */

/* ------------------------------------------------------------- */
/*  Storage for the first PERIOD terms of the sequence         */
/* ------------------------------------------------------------- */
static bool seq_ready = false;
static seq_t seq[PERIOD];

/* ------------------------------------------------------------- */
/*  Compute the coefficients of Φ_943(x) by long division       */
/* ------------------------------------------------------------- */
static void compute_phi_coeffs(void)
{
    if (coeffs_ready)
        return;

    /* Allocate the dividend and divisor polynomials on the stack */
    seq_t R[DEG_R + 1];
    seq_t D[DEG_D + 1];

    /* Zero initialization */
    for (size_t i = 0; i <= DEG_R; ++i)
        R[i] = (seq_t)0;
    for (size_t i = 0; i <= DEG_D; ++i)
        D[i] = (seq_t)0;

    /* R(x) = x^944 - x^943 - x + 1 */
    R[944] = (seq_t)1;
    R[943] = (seq_t)-1;
    R[1]   = (seq_t)-1;
    R[0]   = (seq_t)1;

    /* D(x) = x^64 - x^41 - x^23 + 1 */
    D[64]  = (seq_t)1;
    D[41]  = (seq_t)-1;
    D[23]  = (seq_t)-1;
    D[0]   = (seq_t)1;

    size_t degR = DEG_R;
    while (degR >= DEG_D)
    {
        seq_t factor = R[degR];              /* leading coefficient */
        size_t shift = degR - DEG_D;         /* power shift of the term */

        coeffs[shift] = factor;              /* store coefficient of x^shift */

        /* Subtract factor * D(x) * x^shift from R(x) */
        for (size_t i = 0; i <= DEG_D; ++i)
        {
            if (D[i] != (seq_t)0)
                R[shift + i] = R[shift + i] - factor * D[i];
        }

        R[degR] = (seq_t)0;                  /* leading term is now zero */

        /* Find next non–zero leading term */
        while (degR > 0 && R[degR] == (seq_t)0)
            --degR;
    }

    /* The constant term of Φ_943 is 1 */
    coeffs[0] = (seq_t)1;

    coeffs_ready = true;
}

/* ------------------------------------------------------------- */
/*  Compute the first PERIOD elements of the sequence            */
/* ------------------------------------------------------------- */
static void compute_sequence(void)
{
    if (seq_ready)
        return;

    compute_phi_coeffs();          /* ensure coefficients are ready */

    seq[0] = (seq_t)1;             /* a(0) = 1 */

    for (size_t n = 1; n < PERIOD; ++n)
    {
        seq_t acc = (seq_t)0;
        size_t kmax = (n <= DEG_PHI) ? n : DEG_PHI;
        for (size_t k = 1; k <= kmax; ++k)
            acc = acc + coeffs[k] * seq[n - k];
        seq[n] = -acc;
    }

    seq_ready = true;
}

/* ------------------------------------------------------------- */
/*  Main sequence function                                       */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The sequence is periodic with period PERIOD */
    idx_t idx = n % PERIOD;

    /* Ensure precomputation has been performed */
    compute_sequence();

    return seq[idx];
}
