/*
 * A015389  Inverse of 1380th cyclotomic polynomial.
 * https://oeis.org/A015389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n-th coefficient of the power series expansion of
 *            1/Φ1380(x)
 * Range:     i8 n <= 2000, i16 n <= 2000, i32 n <= 2000, i64 n <= 2000,
 *            i128 n <= 2000, big any n
 * Validated: standard, pass, 97 terms through n = 1966 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the formal inverse of the 1380‑th cyclotomic polynomial.      */
/* The sequence is periodic with period 1380; this implementation precomputes
   the first 1380 coefficients once and then returns the requested term. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* ------------------------------------------------------------------------- */
static void _divisors_uint64(idx_t N, idx_t *darr, size_t *dcount)
{
    size_t cnt = 0;
    for (idx_t i = 1; (i * i) <= N; ++i) {
        if (N % i == 0) {
            darr[cnt++] = i;
            if (i != N / i)
                darr[cnt++] = N / i;
        }
    }
    /* Simple bubble sort (dcount < 64 for N = 1380). */
    for (size_t a = 0; a < cnt; ++a)
        for (size_t b = a + 1; b < cnt; ++b)
            if (darr[a] > darr[b]) {
                idx_t t = darr[a];
                darr[a] = darr[b];
                darr[b] = t;
            }
    *dcount = cnt;
}

/* ------------------------------------------------------------------------- */
static void _poly_divide_monic_in_place(seq_t *num, size_t *num_deg,
                                        const seq_t *den, size_t den_deg)
{
    if (den_deg == 0) return; /* division by constant not needed for this use case */
    int nd = (int)(*num_deg);
    int dd = (int)den_deg;
    size_t qdeg = (nd >= dd) ? (size_t)(nd - dd) : 0;

    /* Temporary array for quotient */
    seq_t *q = mem_new(seq_t, qdeg + 1);

    for (int i = nd; i >= dd; --i) {
        seq_t coef = num[i];
        if (coef == 0) continue;
        size_t pos = (size_t)(i - dd);
        q[pos] = coef;  /* quotient coefficient */
        /* subtract coef * den from num at positions pos .. pos+dd */
        for (size_t j = 0; j <= dd; ++j) {
            num[pos + j] -= coef * den[j];
        }
    }

    /* copy quotient back into num (lower part of array) */
    for (size_t i = 0; i <= qdeg; ++i) {
        num[i] = q[i];
    }
    /* Update degree */
    *num_deg = qdeg;

    mem_free(q);
}

/* ------------------------------------------------------------------------- */
static const seq_t *
_fetch_phi_1380(void)
{
    static seq_t *phi = NULL;          /* pointer to Φ1380 coefficients */
    static size_t phi_deg = 0;         /* degree of Φ1380 */
    if (phi != NULL)
        return phi;                    /* already computed */

    /* ---- 1. List all divisors of 1380 ---------------------------------- */
    const idx_t TARGET = 1380;
    idx_t divs[64];
    size_t dcnt;
    _divisors_uint64(TARGET, divs, &dcnt);

    /* ---- 2. Allocate storage for all Φ_d -------------------------------- */
    seq_t *phis[64];
    size_t degs[64];
    for (size_t i = 0; i < dcnt; ++i) {
        phis[i] = NULL;
        degs[i] = 0;
    }

    /* ---- 3. Compute Φ_d for each divisor in ascending order ------------- */
    for (size_t idx = 0; idx < dcnt; ++idx) {
        idx_t d = divs[idx];
        if (d == 1) { /* Φ1(x) = x - 1 */
            phis[idx] = mem_new(seq_t, 2);
            phis[idx][0] = -(seq_t)1;    /* constant term */
            phis[idx][1] =  (seq_t)1;    /* x term */
            degs[idx] = 1;
            continue;
        }

        /* Numerator: x^d - 1 */
        seq_t *num = mem_new(seq_t, (size_t)d + 1);
        num[0] = -(seq_t)1;              /* constant term -1 */
        num[(size_t)d] =  (seq_t)1;      /* leading term 1 */
        size_t num_deg = (size_t)d;

        /* Divide by all previous Φ_e with e|d, e<d */
        for (size_t j = 0; j < idx; ++j) {
            if (d % divs[j] == 0) {
                _poly_divide_monic_in_place(num, &num_deg,
                                            phis[j], degs[j]);
            }
        }

        /* Store result */
        phis[idx] = num;
        degs[idx] = num_deg;
    }

    /* ---- 4. Extract φ1380 --------------------------------------------- */
    for (size_t i = 0; i < dcnt; ++i) {
        if (divs[i] == TARGET) {
            phi = phis[i];
            phi_deg = degs[i];
            break;
        }
    }

    /* ---- 5. Free unused φ_d ------------------------------------------- */
    for (size_t i = 0; i < dcnt; ++i) {
        if (phis[i] != phi)
            mem_free(phis[i]);
    }

    return phi;
}

/* ------------------------------------------------------------------------- */
static void _build_inverse_series(seq_t *inv)
{
    const size_t L = 1380;

    /* Get Φ1380 coefficients and its degree */
    const seq_t *phi = _fetch_phi_1380();
    const size_t phi_deg = 352;  /* known degree of Φ1380 */

    /* Initial inverse: a_0 = 1 / φ(0) = 1 */
    inv[0] = (seq_t)1;
    size_t cur = 1;  /* number of known coefficients */

    /* Temporary arrays reused each Newton step */
    seq_t *C   = mem_new(seq_t, L);
    seq_t *D   = mem_new(seq_t, L);
    seq_t *new_inv = mem_new(seq_t, L);

    while (cur < L) {
        size_t next = cur << 1;
        if (next > L) next = L;

        /* 1) C = φ * inv (mod x^next) */
        for (size_t i = 0; i < next; ++i) C[i] = 0;
        for (size_t i = 0; i <= phi_deg && i < next; ++i) {
            if (phi[i] == 0) continue;
            for (size_t j = 0; j < cur && i + j < next; ++j) {
                C[i + j] += phi[i] * inv[j];
            }
        }

        /* 2) D = 2 - C (mod x^next) */
        D[0] = (seq_t)2 - C[0];
        for (size_t i = 1; i < next; ++i)
            D[i] = -C[i];

        /* 3) new_inv = inv * D (mod x^next) */
        for (size_t i = 0; i < next; ++i) new_inv[i] = 0;
        for (size_t i = 0; i < cur && i < next; ++i) {
            if (inv[i] == 0) continue;
            for (size_t j = 0; j < next && i + j < next; ++j) {
                new_inv[i + j] += inv[i] * D[j];
            }
        }

        /* 4) copy new_inv back to inv and update length */
        for (size_t i = 0; i < next; ++i)
            inv[i] = new_inv[i];
        cur = next;
    }

    mem_free(C);
    mem_free(D);
    mem_free(new_inv);
}

/* ------------------------------------------------------------------------- */
/* Helper: compute all divisors of N (here N = 1380) and sort them.          */

/* ------------------------------------------------------------------------- */
/* Divide monic polynomial 'num' by monic polynomial 'den', modifying 'num'
   to contain the quotient.  After division, *num_deg is updated.             */

/* ------------------------------------------------------------------------- */
/* Compute Φ1380(x) by the recursive definition:                           */
/* Φ_n(x) = (x^n – 1) / ∏_{d|n, d < n} Φ_d(x).  All polynomials are stored
   with degree increasing: coefficient[0] = constant term.                  */

/* ------------------------------------------------------------------------- */
/* Compute the first 1380 coefficients of the series 1/Φ1380(x).              */
/* Uses Newton--Raphson iteration for power‑series inversion.                 */

/* ------------------------------------------------------------------------- */
/* Public function required by the build system.                               */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static seq_t cache[1380];
    static bool computed = false;
    if (!computed) {
        _build_inverse_series(cache);
        computed = true;
    }
    size_t idx = (size_t)(n % 1380);
    return cache[idx];
}
