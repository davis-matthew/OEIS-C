/*
 * A015493  Inverse of 1484th cyclotomic polynomial.
 * https://oeis.org/A015493
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

/* Inverse of the 1484th cyclotomic polynomial.
   The sequence is periodic with period 1484.
   a(n) = – coefficient of xⁿ in Q(x) = (x¹⁴⁸⁴ – 1)/Φ₁₄₈₄(x),
   where Φ₁₄₈₄(x) is the 1484‑th cyclotomic polynomial.
   We compute the coefficients of Q once and cache them. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define N        1484            /* period length */
#define MAX_DEG  N              /* maximum degree needed */

/* cache for one period */
static seq_t a_cache[N];
static bool  initialized = false;

/* intermediate polynomial tables – use 32‑bit ints to avoid overflow */
static int32_t poly_int[N + 1][MAX_DEG + 1];
static int    degPoly[N + 1];

/* integer polynomial division (monic divisor, numDeg >= denDeg) */
static void poly_divide(const int32_t *num, int numDeg,
                        const int32_t *den, int denDeg,
                        int32_t *quot)
{
    int qDeg = numDeg - denDeg;
    for (int i = 0; i <= qDeg; ++i) quot[i] = 0;

    int32_t tmp[MAX_DEG + 1];
    for (int i = 0; i <= numDeg; ++i) tmp[i] = num[i];

    for (int i = numDeg; i >= denDeg; --i) {
        int32_t fac = tmp[i];
        if (fac != 0) {
            quot[i - denDeg] = fac;
            for (int j = 0; j <= denDeg; ++j)
                tmp[i - denDeg + j] -= fac * den[j];
        }
    }
}

/* Build the period table once */
static void init_sequence(void)
{
    /* 1. find all divisors of N (ascending) */
    idx_t divs[N];
    size_t divcnt = 0;
    for (idx_t d = 1; d <= N; ++d)
        if (N % d == 0) divs[divcnt++] = d;

    /* 2. compute Φ_d for every divisor d */
    for (size_t idx = 0; idx < divcnt; ++idx) {
        idx_t d = divs[idx];
        /* start with x^d – 1 */
        for (int i = 0; i <= d; ++i) poly_int[d][i] = 0;
        poly_int[d][0] = -1;
        poly_int[d][d] = 1;
        degPoly[d] = (int)d;

        /* divide by all earlier Φ_e with e | d */
        for (size_t j = 0; j < idx; ++j) {
            idx_t e = divs[j];
            if (d % e == 0) {
                int32_t tmp[MAX_DEG + 1];
                poly_divide(poly_int[d], degPoly[d],
                            poly_int[e], degPoly[e], tmp);
                /* store quotient back as Φ_d */
                int newDeg = degPoly[d] - degPoly[e];
                for (int k = 0; k <= newDeg; ++k) poly_int[d][k] = tmp[k];
                for (int k = newDeg + 1; k <= degPoly[d]; ++k) poly_int[d][k] = 0;
                degPoly[d] = newDeg;
            }
        }
    }

    /* 3. compute Q(x) = (x^N – 1) / Φ_N(x) */
    idx_t d = N;
    int32_t num[MAX_DEG + 1] = {0};
    num[0] = -1;
    num[N] = 1;
    int numDeg = N;

    int32_t quot[MAX_DEG + 1];
    poly_divide(num, numDeg, poly_int[d], degPoly[d], quot);
    int degQ = numDeg - degPoly[d];

    /* 4. fill cache */
    for (int i = 0; i < N; ++i)
        a_cache[i] = (i <= degQ) ? (seq_t)(-quot[i]) : 0;

    initialized = true;
}

/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    return a_cache[n % N];
}
