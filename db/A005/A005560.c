/*
 * A005560  Number of walks on square lattice. Column y=2 of A052174.
 * https://oeis.org/A005560
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = C(m+3, ceiling(m/2))*C(m+2, floor(m/2)) - C(m+3,
 *            ceiling((m-1)/2))*C(m+2, floor((m-1)/2)), where m = n-2
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 980 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t m;
    seq_t t1, t2;
    if (n < 2) return 0;
    m = n - 2;
    if (m == 0) return 1;
    t1 = binomial(m + 3, (m + 1) / 2) * binomial(m + 2, m / 2);
    t2 = binomial(m + 3, m / 2) * binomial(m + 2, (m - 1) / 2);
    return t1 - t2;
}
