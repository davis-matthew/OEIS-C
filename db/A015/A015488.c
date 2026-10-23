/*
 * A015488  Inverse of 1479th cyclotomic polynomial.
 * https://oeis.org/A015488
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
 * A015488: Inverse of the 1479th cyclotomic polynomial.
 * Periodic with period 1479.
 * We compute the coefficients of Φ1479(x) via 
 *   Φ1479(x) = Π_{d|1479} (x^d - 1)^{μ(1479/d)}.
 * Then a(0)=1 and for n≥1
 *   a(n) = -∑_{k=1}^{degΦ} coeff[k] * a(n-k).
 * The recurrence produces the periodic sequence.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define PERIOD    1479
#define NUM_DIVS  4
#define DEN_DIVS  4
#define MAX_DEG   1528          /* maximum degree of numerator or denominator */
#define DEG       896            /* φ(1479) */

/* Allocated once, reused on every call */
static bool initialized = false;
static seq_t Φcoeff[DEG + 1];
static seq_t pattern[PERIOD];
static seq_t N[MAX_DEG + 1];
static seq_t D[MAX_DEG + 1];
static seq_t Q[MAX_DEG + 1];

/* Multiply polynomial poly[0..*deg] by (x^e - 1), inplace.
   New degree = *deg + e.  Uses a local temporary buffer. */
static void mult_by_factor_inplace(seq_t *poly, size_t *deg, size_t e)
{
    size_t olddeg = *deg;
    size_t newdeg = olddeg + e;
    /* Temporary buffer to hold the new coefficients. */
    seq_t temp[MAX_DEG + 1] = {0};

    for (size_t k = 0; k <= newdeg; ++k) {
        seq_t left  = (k >= e && k - e <= olddeg) ? poly[k - e] : 0;
        seq_t right = (k <= olddeg) ? poly[k] : 0;
        temp[k] = left - right;
    }
    for (size_t k = 0; k <= newdeg; ++k)
        poly[k] = temp[k];
    *deg = newdeg;
}

/* One‑time initialization: compute Φ1479(x) coefficients and the pattern. */
static void init(void)
{
    if (initialized)
        return;
    initialized = true;

    /* ----------------- Build numerator N(x) = Π_{μ=+1} (x^d - 1) ----------------- */
    size_t degN = 0;
    N[0] = 1;
    const size_t num_divs[NUM_DIVS] = {1479, 29, 17, 3};
    for (size_t i = 0; i < NUM_DIVS; ++i)
        mult_by_factor_inplace(N, &degN, num_divs[i]);

    /* ----------------- Build denominator D(x) = Π_{μ=-1} (x^d - 1) ------------------ */
    size_t degD = 0;
    D[0] = 1;
    const size_t den_divs[DEN_DIVS] = {1, 51, 87, 493};
    for (size_t i = 0; i < DEN_DIVS; ++i)
        mult_by_factor_inplace(D, &degD, den_divs[i]);

    /* ----------------- Long division N(x) ÷ D(x) → Φ1479(x) ----------------------- */
    size_t degQ = degN - degD;              /* degree of the quotient */
    for (size_t i = 0; i <= degQ; ++i)
        Q[i] = 0;

    for (size_t i = degN; ; ) {
        seq_t coef = N[i];
        if (coef != 0) {
            size_t qpos = i - degD;
            Q[qpos] = coef;
            /* subtract coef * D(x) shifted by qpos from N */
            for (size_t j = 0; j <= degD; ++j)
                N[j + qpos] -= coef * D[j];
        }
        if (i == degD)
            break;
        --i;
    }

    /* Copy the Φ coefficients to Φcoeff[0..DEG] */
    for (size_t i = 0; i <= DEG; ++i)
        Φcoeff[i] = Q[i];

    /* ----------------- Build the periodic pattern a(0..PERIOD-1) ------------------- */
    pattern[0] = 1;
    for (size_t n = 1; n < PERIOD; ++n) {
        seq_t sum = 0;
        size_t max_i = (n <= DEG) ? n : DEG;
        for (size_t i = 1; i <= max_i; ++i)
            sum += Φcoeff[i] * pattern[n - i];
        pattern[n] = -sum;
    }
}

/* Public accessor */
seq_t a(idx_t n)
{
    init();
    return pattern[(size_t)(n % PERIOD)];
}
