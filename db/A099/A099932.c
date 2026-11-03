/*
 * A099932  Triangle read by rows: nonzero coefficients of polynomials
 *          2^n*E(n,x), with E the Euler polynomials.
 * https://oeis.org/A099932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) = coefficient in row k of the flattened list of non‑zero
 *            coefficients of 2^k * E_k(x), where E_k(x) are Euler
 *            polynomials. Equivalent combinatorial formula: a(n) = binom(k,
 *            j) * num_j * 2^(k-j) with j chosen so that the k‑th non‑zero
 *            term in row k is obtained and num_j are integers defined b...
 * Range:     i8 n <= 15, i16 n <= 32, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 42 (2026-11-03)
 * Helpers:   binomial, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

static inline idx_t count_terms_upto(idx_t r)
/* number of terms in the triangle up to and including row r */
{
    idx_t m   = r + 1;              /* number of rows counted */
    idx_t q   = m >> 1;             /* m/2 */
    idx_t sum_floor;
    if ((m & 1) == 0)               /* m even: m = 2q   */
        sum_floor = q * q;          /* sum_{t=1}^{2q}⌊t/2⌋ = q² */
    else                            /* m odd : m = 2q+1 */
        sum_floor = q * (q + 1);    /* sum_{t=1}^{2q+1}⌊t/2⌋ = q(q+1) */
    return m + sum_floor;           /* 1 per row + sum_floor */
}

/* ---------- helpers for determining the row ----------------- */

static idx_t find_row(idx_t n)
/* return the row k that contains the flattened index n */
{
    idx_t r = 0;
    while (count_terms_upto(r) <= n)
        ++r;
    return r;
}

/* ---------- cache for num_j values -------------------------- */

static seq_t *num_arr  = NULL;   /* array holding num_j integers */
static idx_t  num_len  = 0;      /* current allocated length (elements 0 … num_len-1) */

static void ensure_num_upto(idx_t upto)
/* make sure num_arr[0..upto] are computed */
{
    if (num_len > upto)
        return;
    size_t oldlen = num_len;
    size_t newlen = upto + 1;
    num_arr = mem_resize(seq_t, num_arr, newlen);
    for (size_t i = oldlen; i <= upto; ++i) {
        if (i == 0) {                /* base case */
            num_arr[0] = 1;          /* num_0 = 1 */
            continue;
        }
        idx_t k = (idx_t)i;
        seq_t S = 0;
        for (idx_t t = 0; t < k; ++t) {
            seq_t term = binomial(k, t);
            term *= num_arr[t];
            term *= pow2(k - t);
            S += term;
        }
        num_arr[i] = -(S) / 2;        /* integer division truncates exactly */
    }
    num_len = newlen;
}

/* ---------- main sequence function -------------------------- */

seq_t a(idx_t n)
{
    /* find the row that contains index n */
    idx_t row = find_row(n);

    /* number of terms before this row */
    idx_t cum_before = (row == 0) ? 0 : count_terms_upto(row - 1);

    /* offset within the row (0‑based) */
    idx_t k_offset = n - cum_before;

    /* actual column j in the polynomial coefficients */
    idx_t j = (k_offset == 0) ? 0 : (2 * k_offset - 1);

    /* ensure num_j is available */
    ensure_num_upto(j);

    /* compute the term:
         a(n) = binom(row, j) * num_j * 2^(row - j)   */
    seq_t result = binomial(row, j);
    result *= num_arr[j];
    result *= pow2(row - j);
    return result;
}
