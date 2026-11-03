/*
 * A140356  Triangle T(n,m) read by rows: m! if m <= floor(n/2), and (n-m)!
 *          otherwise.
 * https://oeis.org/A140356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-01)
 * Helpers:   triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A140356: Triangle T(n,m) read by rows.
   a(n) = T(r, m) where r is the row containing the n‑th entry in
   the flattened triangle (0‑based) and m is the column index in that row. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/triangular.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row r such that offset_row(r) <= n < offset_row(r+1). */
    idx_t r = 0;
    seq_t n_seq = (seq_t)n;
    for (;;) {
        seq_t cur_start   = triangular(r);
        seq_t next_start  = triangular(r + 1);
        if (n_seq < next_start)
            break;
        r++;
    }

    /* Column index within the row. */
    seq_t row_start = triangular(r);
    seq_t m_seq     = n_seq - row_start;
    idx_t m = (idx_t)m_seq;

    /* T(r, m): m! if m <= floor(r/2), else (r-m)! */
    idx_t k = (m <= r / 2) ? m : (r - m);
    return factorial(k);
}
