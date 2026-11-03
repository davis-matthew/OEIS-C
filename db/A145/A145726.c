/*
 * A145726  Expansion of q * psi(-q) * psi(-q^15) / (psi(-q^3) * psi(-q^5)) in
 *          powers of q where psi() is a Ramanujan theta function.
 * https://oeis.org/A145726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of q * ψ(-q) * ψ(-q¹⁵) / (ψ(-q³) * ψ(-q⁵)) where ψ(q)
 *            = Σ q^{n(n+1)/2} (n≥0). ψ(-q) means replace q by -q in the
 *            series, i.e. ψ(-q) = Σ (-1)^{n(n+1)/2} q^{n(n+1)/2}
 * Range:     i8 n <= 97, i16 n <= 277, i32 n <= 865, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 861 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Compute the series of ψ(-q^m) up to degree N.
   The i‑th element of the output array holds the coefficient of q^i. */
static void psi_neg_series(idx_t m, size_t N, seq_t *arr)
{
    for (size_t i = 0; i <= N; ++i) arr[i] = 0;
    arr[0] = 1;                     /* n = 0 contributes q^0 */
    idx_t n = 1;
    while (1) {
        idx_t deg = m * n * (n + 1) / 2;
        if (deg > N) break;
        /* sign = (-1)^{n(n+1)/2} */
        seq_t sign = ((n * (n + 1) / 2) & 1) ? -1 : 1;
        arr[deg] += sign;
        ++n;
    }
}

/* Compute the multiplicative inverse of a series h (with h[0]==1) up to degree N. */
static void series_inv(const seq_t *h, size_t N, seq_t *inv)
{
    inv[0] = 1;  /* invariant */
    for (size_t i = 1; i <= N; ++i) {
        seq_t sum = 0;
        for (size_t j = 1; j <= i; ++j) {
            if (h[j] != 0) sum += h[j] * inv[i - j];
        }
        inv[i] = -sum;    /* division by 1 */
    }
}

/* Multiply two series a and b, storing the result in res, truncated to degree N. */
static void series_mul(const seq_t *a, const seq_t *b,
                       seq_t *res, size_t N)
{
    for (size_t i = 0; i <= N; ++i) res[i] = 0;
    for (size_t i = 0; i <= N; ++i) {
        if (a[i] == 0) continue;
        size_t maxj = N + 1 - i;
        for (size_t j = 0; j < maxj; ++j) {
            if (b[j] == 0) continue;
            res[i + j] += a[i] * b[j];
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache */
    static seq_t *psi1   = NULL;
    static seq_t *psi15  = NULL;
    static seq_t *psi3   = NULL;
    static seq_t *psi5   = NULL;
    static seq_t *den    = NULL;
    static seq_t *invden = NULL;
    static seq_t *num    = NULL;
    static seq_t *shift  = NULL;
    static seq_t *final  = NULL;
    static size_t cache_size = 0;

    size_t N = (size_t)n;          /* need coefficients 0..N inclusive */

    if (N > cache_size) {
        /* free old arrays */
        mem_free(psi1);  mem_free(psi15);  mem_free(psi3);  mem_free(psi5);
        mem_free(den);   mem_free(invden); mem_free(num);   mem_free(shift); mem_free(final);

        /* allocate new arrays */
        psi1   = mem_new(seq_t, N + 1);
        psi15  = mem_new(seq_t, N + 1);
        psi3   = mem_new(seq_t, N + 1);
        psi5   = mem_new(seq_t, N + 1);
        den    = mem_new(seq_t, N + 1);
        invden = mem_new(seq_t, N + 1);
        num    = mem_new(seq_t, N + 1);
        shift  = mem_new(seq_t, N + 1);
        final  = mem_new(seq_t, N + 1);

        /* compute the four ψ series */
        psi_neg_series(1,  N, psi1);    /* ψ(-q)   */
        psi_neg_series(15, N, psi15);  /* ψ(-q¹⁵) */
        psi_neg_series(3,  N, psi3);    /* ψ(-q³)  */
        psi_neg_series(5,  N, psi5);    /* ψ(-q⁵)  */

        /* denominator = ψ(-q³) * ψ(-q⁵) */
        series_mul(psi3, psi5, den, N);

        /* inverse of denominator */
        series_inv(den, N, invden);

        /* numerator = ψ(-q) * ψ(-q¹⁵) */
        series_mul(psi1, psi15, num, N);

        /* shift numerator by q (multiply by q) */
        shift[0] = 0;
        for (size_t i = 0; i < N; ++i) shift[i + 1] = num[i];

        /* final series = (q * ψ(-q) * ψ(-q¹⁵)) / (ψ(-q³) * ψ(-q⁵)) */
        series_mul(shift, invden, final, N);

        cache_size = N;
    }

    return final[n];
}
