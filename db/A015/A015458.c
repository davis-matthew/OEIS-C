/*
 * A015458  Inverse of 1449th cyclotomic polynomial.
 * https://oeis.org/A015458
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

/* Sequence A015458: coefficients of the reciprocal of the 1449‑th cyclotomic polynomial.
   The coefficients are periodic with period 1449.  The function computes a(n) for all
   n >= 0 by generating the full period once.  The computation is based on the
   defining relation Φ₁₄₄₉(x)·(∑_{i≥0}a(i)x^i)=1, where Φ₁₄₄₉ is the 1449‑th
   cyclotomic polynomial.  First Φ₁₄₄ₙ is constructed by multiplying/dividing
   binomials (x^k‑1) according to the Möbius transform, then the recurrence
   a(0)=1, a(m)=−∑_{k=1}^{min(m,φ)}c_k a(m‑k) (c_k being the coefficients of
   Φ₁₄₄₉) yields the period. */

#include <stdint.h>
#include <stddef.h>

#define N      1449U          /* cyclotomic order */
#define PERIOD N
#define MAX_POLY_DEG 2000     /* large enough for intermediate product */

/* multiply current polynomial P by (x^k - 1)
   P is stored in array poly with current length *len.
   After multiplication length becomes *len + k. */
static void multiply_by_shift_minus1(seq_t *poly, size_t *len, idx_t k)
{
    size_t old_len = *len;
    seq_t tmp[MAX_POLY_DEG + 1] = {0};
    size_t i;
    for (i = 0; i < old_len; ++i) {
        tmp[i] -= poly[i];
        tmp[i + k] += poly[i];
    }
    for (i = 0; i < old_len + k; ++i)
        poly[i] = tmp[i];
    *len = old_len + k;
}

/* divide polynomial P (length *len) by (x^k - 1).
   The result overwrites the first *out_len entries of poly. */
static void divide_by_shift_minus1(seq_t *poly, size_t len, idx_t k,
                                   size_t *out_len)
{
    size_t degP = len - 1;
    size_t degQ = degP - (size_t)k;
    seq_t q[MAX_POLY_DEG + 1] = {0};
    size_t i;
    for (i = 0; i <= degQ; ++i) {
        if (i < (size_t)k)
            q[i] = -poly[i];
        else
            q[i] = q[i - k] - poly[i];
    }
    for (i = 0; i <= degQ; ++i)
        poly[i] = q[i];
    *out_len = degQ + 1;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    static int initialized = 0;
    static seq_t cache[PERIOD];
    if (!initialized) {
        /* ---------- build Φ_N(x) ---------- */
        seq_t poly[MAX_POLY_DEG + 1] = {0};
        size_t poly_len = 1;
        poly[0] = 1;                         /* start with 1 */

        /* positive Möbius contributions:  μ = +1 for d = 1,21,69,161 */
        const idx_t pos_ds[] = {1U, 21U, 69U, 161U};
        size_t i;
        for (i = 0; i < 4; ++i) {
            idx_t d = pos_ds[i];
            idx_t k = N / d;
            multiply_by_shift_minus1(poly, &poly_len, k);
        }

        /* negative Möbius contributions:  μ = –1 for d = 3,7,23,411 */
        const idx_t neg_ds[] = {3U, 7U, 23U, 411U};
        for (i = 0; i < 4; ++i) {
            idx_t d = neg_ds[i];
            idx_t k = N / d;
            size_t new_len;
            divide_by_shift_minus1(poly, poly_len, k, &new_len);
            poly_len = new_len;
        }

        /* copy Φ_N coefficients into a temporary array */
        seq_t phi[poly_len];
        for (i = 0; i < poly_len; ++i)
            phi[i] = poly[i];               /* phi[0] == 1 */

        /* ---------- compute the period of 1/Φ_N(x) ---------- */
        cache[0] = 1;                       /* a(0) = 1 */
        for (idx_t m = 1; m < PERIOD; ++m) {
            seq_t sum = 0;
            idx_t maxk = (m < (idx_t)(poly_len - 1)) ? m : (idx_t)(poly_len - 1);
            idx_t k;
            for (k = 1; k <= maxk; ++k)
                sum += phi[k] * cache[m - k];
            cache[m] = -sum;
        }
        initialized = 1;
    }
    return cache[n % PERIOD];
}
