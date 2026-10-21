/*
 * A093127  Triangle read by rows: differences of Narayana numbers.
 * https://oeis.org/A093127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   T(n,k) = Narayana(n-k+2,k+1) - Narayana(n-k+1,k) * where
 *            Narayana(m,k) = binomial(m,k)*binomial(m,k-1)/m * a(n) is
 *            obtained by reading the triangle by rows
 * Range:     u8 n <= 26, u16 n <= 75, u32 n <= 216, u64 n <= 715,
 *            u128 n <= 2574, big any n
 * Helpers:   cum_inc, narayana
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cum_inc.h"
#include "../utils/narayana.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* locate the row r that contains index n */
    idx_t r = 0;
    while (cum_inc(r) <= (seq_t)n)
        ++r;

    /* start index of this row (cumulative up to previous row) */
    seq_t prev = (r == 0) ? 0 : cum_inc(r - 1);

    /* column index k within the row (0‑based) */
    seq_t k_seq = (seq_t)n - prev;
    idx_t k = (idx_t)k_seq;

    /* compute the triangle entry T(r,k) */
    seq_t t1 = narayana(r - k + 2, k + 1);
    seq_t t2 = narayana(r - k + 1, k);
    return t1 - t2;
}
