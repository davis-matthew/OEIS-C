/*
 * A188816  Triangle read by rows: row n gives (coefficients * (n-1)!) in
 *          expansion of pieces k=0..n-1 of the probability mass function for
 *          the Irwin-Hall distribution, lowest powers first.
 * https://oeis.org/A188816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, tabf
 * Formula:   a(n) is the coefficient (n‑1)! times the polynomial expansion of
 *            the k‑th piece of the Irwin–Hall distribution for n variables.
 *            The coefficient is a(n) = C(n‑1,c) * Σ_{j=0}^{k} (−1)^{j+e} *
 *            C(n,j) * j^e where e = n‑1‑c, n = row number (1‑based), k =
 *            piece index within the row (0‑based), c = coeff...
 * Range:     i8 n <= 40, i16 n <= 113, i32 n <= 346, i64 n <= 1416,
 *            i128 n <= 6633, big any n
 * Helpers:   binom_n_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_n_k.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t power  = base;
    while (exp) {
        if (exp & 1) result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t N)
{
    if (N == 0) return 0;          /* safety for potential out‑of‑bounds call */
    idx_t pos = N - 1;              /* 0‑based position in the flattened triangle */

    /* Find row r such that S(r‑1) ≤ pos < S(r),
       where S(r) = r(r+1)(2r+1)/6  (sum of squares). */
    idx_t lo = 1, hi = 1;
    while (true) {
        __int128 h = hi;
        __int128 S_hi = h * (h + 1) * (2 * h + 1) / 6;
        if (S_hi > pos) break;
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 m = mid;
        __int128 S_mid = m * (m + 1) * (2 * m + 1) / 6;
        if (S_mid > pos) hi = mid;
        else lo = mid + 1;
    }
    idx_t r = lo;               /* row number (1‑based) */

    /* Position within the row */
    idx_t r_minus1 = r - 1;
    __int128 rm1 = r_minus1;
    __int128 S_r_minus1 = rm1 * (rm1 + 1) * (2 * rm1 + 1) / 6;
    idx_t offset = pos - (idx_t)S_r_minus1;    /* 0‑based offset in row */
    idx_t piece = offset / r;                  /* k */
    idx_t coeff_idx = offset % r;              /* c */

    idx_t e = r_minus1 - coeff_idx;            /* exponent n‑1‑c */

    /* Sum over j = 0 .. piece */
    seq_t sum = 0;
    seq_t binom = 1;                           /* C(r,0) */
    for (idx_t j = 0; j <= piece; ++j) {
        bool negative = ((j + e) & 1) ? true : false;
        seq_t term;
        if (j == 0 && e == 0) {
            term = 1;                          /* 0^0 is treated as 1 */
        } else {
            seq_t pow_j = pow_seq((seq_t)j, e);
            term = binom * pow_j;
        }
        if (negative) sum -= term;
        else          sum += term;

        /* Prepare binomial for next j */
        if (j < piece) {
            binom = binom * (r - j);
            binom = binom / (j + 1);
        }
    }

    seq_t binom_n1_c = binom_n_k(r - 1, coeff_idx);
    return binom_n1_c * sum;
}
