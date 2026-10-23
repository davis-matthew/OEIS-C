/*
 * A014802  Inverse of 793rd cyclotomic polynomial.
 * https://oeis.org/A014802
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
#include <stddef.h>

/* --------------------------------------------------------------------
   OEIS A014802  
   Formula: a(n)  is the coefficient of x^n in the formal power series
   expansion 1 / Φ₇₉₃(x), where Φ₇₉₃(x) is the 793rd cyclotomic
   polynomial.  The sequence is periodic with period 793.
   -------------------------------------------------------------------- */

 /* Constants (values are compile‑time constants so they work in C and C++) */
 enum { DEGREED_N = 794,                    /* degree of N(x) = (x^793−1)(x−1) */
        DEGREED_D = 74,                     /* degree of D(x) = (x^13−1)(x^61−1) */
        DEGREED_Q = 720,                    /* degree of Φ₇₉₃(x) = N/D  */
        PERIOD      = 793 };

/* Static cache for one period of the sequence */
static seq_t period_cache[PERIOD];
static bool period_initialized = false;

/* Build the period cache once --------------------------------------- */
static void init_period(void)
{
    /* Polynomials are stored as arrays of coefficients, index = exponent */
    seq_t N[DEGREED_N + 1] = {0};
    seq_t D[DEGREED_D + 1] = {0};
    seq_t Q[DEGREED_Q + 1] = {0};

    /* N(x) = (x^793−1)(x−1) = x^794 − x^793 − x + 1 */
    N[0]   =  1;
    N[1]   = -1;
    N[793] = -1;
    N[794] =  1;

    /* D(x) = (x^13−1)(x^61−1) = x^74 − x^13 − x^61 + 1 */
    D[0]  =  1;
    D[13] = -1;
    D[61] = -1;
    D[74] =  1;

    /* Polynomial division N ÷ D → Q (exact division) */
    for (int i = DEGREED_N; i >= DEGREED_D; --i)
    {
        seq_t coeff = N[i];
        if (coeff == 0) continue;
        int shift = i - DEGREED_D;           /* shift amount */

        Q[shift] = coeff;                    /* set coefficient of Q */

        /* subtract coeff * D(x) shifted by 'shift' from N(x) */
        for (int j = 0; j <= DEGREED_D; ++j)
            N[j + shift] -= coeff * D[j];
    }

    /* Compute the power‑series inverse R(x) = 1 / Q(x) (mod x^PERIOD)   */
    seq_t R[PERIOD];
    R[0] = 1;                                /* Q[0] == 1 for cyclotomics */

    /* Recurrence: for k ≥ 1, R[k] = –Σ_{i=1..min(k,DEGREED_Q)} Q[i] * R[k-i] */
    for (int k = 1; k < PERIOD; ++k)
    {
        seq_t sum = 0;
        int imax = (k <= DEGREED_Q) ? k : DEGREED_Q;
        for (int i = 1; i <= imax; ++i)
            sum += R[k - i] * Q[i];
        R[k] = -sum;                         /* because Q[0] == 1 */
    }

    /* Store the computed period in the cache */
    for (int i = 0; i < PERIOD; ++i)
        period_cache[i] = R[i];

    period_initialized = true;
}

/*  OEIS sequence A014802 -------------------------------------------------
    Returns the nth term of the sequence (0‑based indexing).            */
seq_t a(idx_t n)
{
    if (!period_initialized)
        init_period();

    /* n can be very large – reduce modulo PERIOD first */
    size_t index = (size_t)(n % PERIOD);
    return period_cache[index];
}
