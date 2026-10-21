/*
 * A099594  Array read by antidiagonals: poly-Bernoulli numbers B(-k,n).
 * https://oeis.org/A099594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 124, u64 n <= 285,
 *            u128 n <= 755, big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A099594 – Array read by antidiagonals: poly‑Bernoulli numbers B(-k,n)
 *
 * Implementation uses the closed form
 *
 *     T(n,k) = Σ_{j=0..n} (j+1)^k · Σ_{i=0..j} (-1)^{n+j-i} · C(j,i) · (j-i)^n
 *
 *  a(n) is obtained by reading the triangle T(n,k) along its antidiagonals.
 *  This method pre‑computes all necessary values for n,k ≤ 150 (sufficient for
 *  all a(n) with n < 10 000) and then indexes the requested entry.
 */

#include <stdbool.h>   /* for bool */
#include "../utils/antidiagonal_coords.h"

#define NMAX 150

static seq_t Cbuf[NMAX + 1][NMAX + 1];          /* binomial coefficients C(j,i)     */
static seq_t powbuf[NMAX + 2][NMAX + 1];        /* powbuf[d][e] = d^e, 0 ≤ d ≤ NMAX+1, 0 ≤ e ≤ NMAX */
static seq_t factbuf[NMAX + 1];                 /* factorials i!                    */
static seq_t innerbuf[NMAX + 1][NMAX + 1];      /* inner sums   Σ_{i=0..j} (-1)^{n+j-i} C(j,i)(j-i)^n */
static seq_t Tbuf[NMAX + 1][NMAX + 1];          /* triangle values T(n,k)           */

static bool initialized = false;

/* initialise all tables once */
static void init_tables(void)
{
    /* binomial coefficients C(j,i) */
    for (idx_t j = 0; j <= NMAX; ++j) {
        Cbuf[j][0] = 1;
        Cbuf[j][j] = 1;
        for (idx_t i = 1; i < j; ++i) {
            Cbuf[j][i] = Cbuf[j - 1][i - 1] + Cbuf[j - 1][i];
        }
    }

    /* powers d^e (including 0^0 = 1, 0^e>0 = 0) */
    for (idx_t d = 0; d <= NMAX + 1; ++d) {
        powbuf[d][0] = (d == 0) ? 1 : 1;
        if (d == 0) {
            for (idx_t e = 1; e <= NMAX; ++e) powbuf[d][e] = 0;
        } else {
            seq_t p = 1;
            for (idx_t e = 1; e <= NMAX; ++e) {
                p = p * (seq_t)d;
                powbuf[d][e] = p;
            }
        }
    }

    /* factorials */
    factbuf[0] = 1;
    for (idx_t i = 1; i <= NMAX; ++i) factbuf[i] = factbuf[i - 1] * (seq_t)i;

    /* inner sums required for T(n,k) */
    for (idx_t n = 0; n <= NMAX; ++n) {
        for (idx_t j = 0; j <= n; ++j) {
            seq_t pos = 0;
            seq_t neg = 0;
            for (idx_t i = 0; i <= j; ++i) {
                seq_t comb = Cbuf[j][i];
                seq_t pot  = powbuf[j - i][n];
                seq_t term = comb * pot;
                if (((n + j - i) & 1) == 0) pos += term; /* even exponent => + */
                else                         neg += term; /* odd  exponent => - */
            }
            /* pos ≥ neg holds because the whole inner sum is non‑negative */
            innerbuf[n][j] = pos - neg;
        }
    }

    /* triangle T(n,k) by weighted sum of inner values */
    for (idx_t n = 0; n <= NMAX; ++n) {
        for (idx_t k = 0; k <= NMAX; ++k) {
            seq_t total = 0;
            for (idx_t j = 0; j <= n; ++j) {
                seq_t factor = powbuf[j + 1][k];
                seq_t prod   = factor * innerbuf[n][j];
                total += prod;
            }
            Tbuf[n][k] = total;
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_tables();

    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    if (i > NMAX) i = NMAX;  /* safety, should not happen for n <= 10 000 */
    if (j > NMAX) j = NMAX;

    return Tbuf[i][j];
}
