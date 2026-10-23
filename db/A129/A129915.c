/*
 * A129915  Irregular triangle read by rows: T(n, k) = f(n, A113474(n-1) - k),
 *          where f(n, k) = (n-1)!/2^k if (n-1)!/2^k is an integer, otherwise
 *          f(n, k) = 0.
 * https://oeis.org/A129915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th term of the irregular triangle read by rows.
 *            Row r (r ≥ 1) contains w(r) = r – popcount(r–1) entries. The
 *            k-th entry (1 ≤ k ≤ w(r)) is (r–1)! / 2^(w(r)–k). The sequence
 *            is obtained by reading the rows one after another
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 79, u64 n <= 206,
 *            u128 n <= 573, big any n
 * Helpers:   entry_row_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/entry_row_pos.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t idx = n - 1;          /* convert to zero‑based sequence index */

    idx_t r    = 1;             /* current row number (1‑based) */
    idx_t sum  = 0;             /* total number of entries seen so far */

    while (1) {
        /* width of this row: w(r) = r – popcount(r–1) */
        idx_t w = r - __builtin_popcountll(r-1);

        if (sum + w > idx) {    /* target entry lies in this row */
            idx_t pos = idx - sum;      /* zero‑based position within the row */
            return entry_row_pos(r, pos);
        }

        sum += w;
        ++r;
    }
}
