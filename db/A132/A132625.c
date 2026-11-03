/*
 * A132625  Triangle T, read by rows, where row n+1 of T = row n of T^(2^n)
 *          with appended '1' for n>=0 with T(0,0)=1.
 * https://oeis.org/A132625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 27, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A132625
 *  The triangle T is defined recursively by
 *
 *      T(i,i) = 1
 *
 *      For 1 ≤ k ≤ i
 *          T(i,i‑k) = C(2ⁱ, k) – Σ_{s=0}^{k‑1} T(i,i‑s) · C(2^{i‑s}–1, k‑s)
 *
 *  The sequence a(n) is obtained by reading T row by row, starting with
 *  a(0)=T(0,0).  For an index n let
 *          r = ⌊(√(8n+1) – 1)/2⌋         (the row number)
 *          c = n – r(r+1)/2              (the column number)
 *  then a(n) = T(r,c).
 *
 *  The implementation below builds the required rows of T on demand.
 *  It uses the pow2(n) utility from <../utils/pow2.h> and a local
 *  binomial routine that works for arbitrary‐precision seq_t values.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* local binomial: C(a, b)  (a is seq_t, b is idx_t, 0 ≤ b ≤ a) */
static seq_t binom_seq_seq(seq_t a, idx_t b)
{
    if (b == 0) return 1;
    seq_t res = 1;
    for (idx_t t = 1; t <= b; ++t) {
        res = (res * (a - (seq_t)(b - t))) / t;
    }
    return res;
}

/* array of pointers to rows of T; each row occupies (row+1) seq_t elements */
static seq_t **T_rows     = NULL;
static idx_t  T_rows_cap  = 0;
static idx_t  T_rows_cnt  = 0;

/* ensure that rows 0 … max_row are computed */
static void ensure_rows(idx_t max_row)
{
    if (T_rows_cnt > max_row) return;

    /* grow the row pointer array if necessary */
    if (T_rows_cnt >= T_rows_cap) {
        idx_t new_cap = T_rows_cap ? T_rows_cap * 2 : 1;
        T_rows = (seq_t **)realloc(T_rows, new_cap * sizeof(seq_t *));
        T_rows_cap = new_cap;
    }

    for (idx_t i = T_rows_cnt; i <= max_row; ++i) {
        /* allocate the new row */
        seq_t *row = mem_new(seq_t, (i + 1));
        row[i] = 1;                      /* base case T(i,i)=1 */

        seq_t pow2_i = pow2(i);

        for (idx_t k = 1; k <= i; ++k) {
            seq_t binom_left = binom_seq_seq(pow2_i, k);
            seq_t sum = 0;
            for (idx_t s = 0; s < k; ++s) {
                idx_t j = i - s;               /* column index where value is known */
                seq_t pow2_j_minus1 = pow2(j) - 1;
                seq_t binom_mid = binom_seq_seq(pow2_j_minus1, k - s);
                sum += row[j] * binom_mid;
            }
            row[i - k] = binom_left - sum;
        }

        T_rows[i] = row;
    }
    T_rows_cnt = max_row + 1;
}

/* compute the row number r for a given index n */
static inline idx_t row_of(idx_t n)
{
    long double d = sqrt((long double)8 * n + 1);
    idx_t r = (idx_t)((d - 1) / 2);
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    return r;
}

/* main sequence function */
seq_t a(idx_t n)
{
    idx_t r   = row_of(n);
    idx_t col = n - r * (r + 1) / 2;
    ensure_rows(r);
    return T_rows[r][col];
}
