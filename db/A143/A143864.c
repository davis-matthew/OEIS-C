/*
 * A143864  Eigentriangle of A055461 (square subsequences decrescendo).
 * https://oeis.org/A143864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = r^2 when the term is the first in its row * a(n) =
 *            S[c‑1] × (r − c + 1)^2 otherwise * * Here r is the row number
 *            such that r(r‑1)/2 < n ≤ r(r+1)/2, * c is the column number in
 *            that row (1‑based), and * S[m] is the sum of the m‑th row of the
 *            triangle. * * The row sums satisfy: * S[1] = 1...
 * Range:     u8 n <= 24, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

#define MAX_R 2000   /* safe upper bound for r when n ≤ 10 000 (r ≤ 141) */

seq_t a(idx_t n)
{
    /* -------- find the row r and the column c (1‑based) -------- */
    idx_t r = 1;
    while ( (seq_t)r * (r + 1) / 2 < n ) {   /* r grows slowly (≤ 141) */
        ++r;
    }
    idx_t offset = (r - 1) * r / 2;          /* number of terms before row r */
    idx_t c = n - offset;                    /* column in the row */

    /* -------- static cache of row sums S[1..] -------- */
    static seq_t S[MAX_R + 1];
    static idx_t Smax = 0;                   /* largest m whose S[m] is ready */

    if (c > 1 && Smax < (c - 1)) {
        for (idx_t m = Smax + 1; m <= c - 1; ++m) {
            seq_t sum = seq_mul((seq_t)m, (seq_t)m);   /* m² */
            for (idx_t j = 1; j < m; ++j) {
                seq_t j_sq = seq_mul((seq_t)j, (seq_t)j);
                sum += seq_mul(S[m - j], j_sq);
            }
            S[m] = sum;
        }
        Smax = c - 1;
    }

    /* -------- compute the requested term -------- */
    if (c == 1) {
        return seq_mul((seq_t)r, (seq_t)r);            /* r² */
    } else {
        idx_t diff = r - c + 1;                        /* (r − c + 1) */
        seq_t diff_sq = seq_mul((seq_t)diff, (seq_t)diff);
        return seq_mul(S[c - 1], diff_sq);
    }
}
