/*
 * A177810  Triangle binomial(6*n,6*m), 0 <= m <= n, read by rows.
 * https://oeis.org/A177810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   triangle binomial(6*n,6*m), 0 <= m <= n, read by rows
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 22, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"


seq_t a(idx_t n) {
    /* Find row r and column m such that
       cumulative terms before row r <= n < cumulative terms up through row r.
       Row r has r+1 terms. */
    idx_t r = 0;
    idx_t cumulative = 0;          /* terms before the current row */
    while (cumulative + (r + 1) <= n) {
        cumulative += r + 1;       /* add current row length */
        ++r;
    }
    idx_t m = n - cumulative;      /* column index within the found row */
    return binomial(6 * r, 6 * m);
}
