/*
 * A083487  Triangle read by rows: T(n,k) = 2*n*k + n + k (1 <= k <= n).
 * https://oeis.org/A083487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*r*k + r + k, where r = ceil((sqrt(8*n+1) - 1) / 2) (the
 *            row containing n) k = n - r*(r-1)/2 (position within that row)
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   find_row_for_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/find_row_for_index.h"

seq_t a(idx_t n)
{
    idx_t r = find_row_for_index(n);

    /* total terms before row r */
    idx_t prev_terms = (idx_t)((unsigned __int128)(r - 1) * r / 2);

    /* position k within the row (1‑based) */
    idx_t k = n - prev_terms;

    /* compute T(r,k) = 2*r*k + r + k */
    seq_t r_seq = (seq_t)r;
    seq_t k_seq = (seq_t)k;
    seq_t term = 2 * r_seq * k_seq;
    term += r_seq + k_seq;
    return term;
}
