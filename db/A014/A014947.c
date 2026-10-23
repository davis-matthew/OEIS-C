/*
 * A014947  Inverse of 938th cyclotomic polynomial.
 * https://oeis.org/A014947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_divisors_938
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Inverse of the 938th cyclotomic polynomial.
 * a(0) = 1
 * a is periodic with period 938.
 * The sequence is given by 
 *   a(n) = -ψ(n mod 938)
 * where ψ(x) = (x^938 - 1) / Φ_938(x)
 * is the quotient polynomial of dividing x^938-1 by the 938‑th cyclotomic
 * polynomial Φ_938.
 *  
 * The implementation computes Φ_938 exactly by successive divisions by the
 * cyclotomic polynomials of all proper divisors of 938 (1,2,7,14,67,134,469),
 * then divides x^938-1 by Φ_938 to obtain ψ, and finally returns
 * the stored negative coefficient.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_divisors_938.h"

/* ------------------------------------------------------------------ */
/* Helper to get all divisors of 938 in increasing order.            */

/* ------------------------------------------------------------------ */
/* Polynomial division where the divisor has leading coefficient 1.   */
/* ------------------------------------------------------------------ */
static void poly_div_monic(const seq_t *num, size_t nlen,
                           const seq_t *den, size_t dlen,
                           seq_t **qout, size_t *qlen)
{
    *qlen = nlen - dlen + 1;
    seq_t *rem = (seq_t*)calloc(nlen, sizeof(seq_t));
    for (size_t i = 0; i < nlen; ++i) rem[i] = num[i];

    seq_t *q = (seq_t*)calloc(*qlen, sizeof(seq_t));

    for (int ii = (int)nlen - 1; ii >= (int)dlen - 1; --ii)
    {
        seq_t coeff = rem[ii];
        if (coeff != 0)
        {
            size_t qpos = ii - (dlen - 1);
            q[qpos] = coeff;
            for (size_t sj = 0; sj < dlen; ++sj)
                rem[qpos + sj] -= coeff * den[sj];
        }
    }
    free(rem);
    *qout = q;
}

/* ------------------------------------------------------------------ */
/* Compute ψ(x) = (x^938 - 1) / Φ_938(x) and store its coefficients.  */
/* ------------------------------------------------------------------ */
static void init_psi(void)
{
    const idx_t N = 938;
    static seq_t psi[938];                 /* period coefficients    */
    static bool inited = false;

    /* 1. Obtain sorted divisors of 938. */
    idx_t divs[8];
    size_t divc;
    get_divisors_938(divs, &divc);

    /* 2. Store cyclotomic polynomials of all divisors. */
    seq_t *phi[8];
    size_t philen[8];

    for (size_t id = 0; id < divc; ++id)
    {
        idx_t n = divs[id];
        size_t nlen = (size_t)n + 1;          /* degree n polynomial */
        seq_t *poly = (seq_t*)calloc(nlen, sizeof(seq_t));
        poly[0] = -1;                         /* constant term */
        poly[n] = 1;                          /* leading term */

        /* Divide by all Φ_d for proper divisors d|n. */
        for (size_t jd = 0; jd < id; ++jd)
        {
            if (n % divs[jd] != 0) continue;
            seq_t *q; size_t qlen;
            poly_div_monic(poly, nlen, phi[jd], philen[jd], &q, &qlen);
            free(poly);
            poly = q;
            nlen = qlen;
        }
        phi[id]   = poly;
        philen[id] = nlen;
    }

    /* 3. The last divisor is N itself.  φ_N now stored in phi[divc-1]. */
    seq_t *phiN   = phi[divc - 1];
    size_t phiNlen = philen[divc - 1];

    /* 4. Divide x^N - 1 by Φ_N to obtain ψ. */
    size_t numlen = (size_t)N + 1;
    seq_t *num = (seq_t*)calloc(numlen, sizeof(seq_t));
    num[0] = -1;              /* constant term */
    num[N] = 1;               /* leading term */

    seq_t *q; size_t qlen;
    poly_div_monic(num, numlen, phiN, phiNlen, &q, &qlen);

    /* 5. Store ψ coefficients padded with zeros up to period N. */
    for (size_t i = 0; i < N; ++i) psi[i] = 0;
    for (size_t i = 0; i < qlen; ++i) psi[i] = q[i];
    free(q);
    free(num);
    /* Free all intermediate Φ polynomials. */
    for (size_t i = 0; i < divc; ++i) free(phi[i]);

    /* 6. Store globally for fast lookup. */
    static seq_t period_coeff[938];
    for (size_t i = 0; i < N; ++i) period_coeff[i] = psi[i];
    /* Mark init done. */
    inited = true;

    /* Assign to static variable accessible by a(). */
    static bool already_stored = false;
    if (!already_stored)
    {
        static seq_t *period_ptr = NULL;
        period_ptr = period_coeff;
        already_stored = true;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.                       */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Lazy initialization on first call. */
    static bool init_done = false;
    static seq_t period[938];
    if (!init_done)
    {
        /* Compute ψ coefficients first. */
        const idx_t N = 938;
        /* 1. Obtain sorted divisors of 938. */
        idx_t divs[8];
        size_t divc;
        get_divisors_938(divs, &divc);

        /* 2. Compute Φ for all divisors. */
        seq_t *phi[8];
        size_t philen[8];

        for (size_t id = 0; id < divc; ++id)
        {
            idx_t n = divs[id];
            size_t nlen = (size_t)n + 1;
            seq_t *poly = (seq_t*)calloc(nlen, sizeof(seq_t));
            poly[0] = -1;
            poly[n] = 1;

            for (size_t jd = 0; jd < id; ++jd)
            {
                if (n % divs[jd] != 0) continue;
                seq_t *q; size_t qlen;
                poly_div_monic(poly, nlen, phi[jd], philen[jd], &q, &qlen);
                free(poly);
                poly = q;
                nlen = qlen;
            }
            phi[id]   = poly;
            philen[id] = nlen;
        }

        /* 3. Build numerator x^N - 1. */
        seq_t *num = (seq_t*)calloc(N + 1, sizeof(seq_t));
        num[0] = -1; num[N] = 1;

        /* 4. Divide by Φ_N to obtain ψ. */
        seq_t *q; size_t qlen;
        poly_div_monic(num, N + 1, phi[divc - 1], philen[divc - 1], &q, &qlen);
        free(num);

        /* 5. Copy ψ into period array padded with zeros. */
        for (size_t i = 0; i < N; ++i) period[i] = 0;
        for (size_t i = 0; i < qlen; ++i) period[i] = q[i];
        free(q);

        /* 6. Free all Φ polynomials. */
        for (size_t i = 0; i < divc; ++i) free(phi[i]);

        init_done = true;
    }

    idx_t mod = n % 938;
    /* Return negative of the ψ coefficient. */
    return -period[mod];
}
