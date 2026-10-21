/*
 * A201143  Irregular triangular array read by rows T(n,k) is the number of
 *          2-colored labeled graphs that have exactly k edges, n >= 2, 0 <= k
 *          <= A033638(n).
 * https://oeis.org/A201143
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = Σ_{a=1}^{⌊(n-1)/2⌋} binom(n,a)*binom(a*(n-a),k) + (n
 *            even? binom(n,n/2)/2 * binom((n/2)²,k) : 0) a(n) is the
 *            flattened triangle read by rows with offset 2: a(n) = T(r,k)
 *            where r is the row containing the nth term and k is its 0‑based
 *            column index
 * Range:     u8 n <= 20, u16 n <= 45, u32 n <= 114, u64 n <= 289,
 *            u128 n <= 888, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* indices below the offset are irrelevant */
    if (n < 2) return 0;

    /* locate the row r that contains the nth term.
       rows start at n = 2 with length floor(r²/4)+1. */
    idx_t start = 2;   /* first index of the current row */
    idx_t r = 2;       /* current row number */

    while (1) {
        idx_t len = (r*r)/4 + 1;
        if (n < start + len) break;  /* n lies in this row */
        start += len;
        ++r;
    }

    idx_t k = n - start;   /* column index (0‑based) */

    seq_t sum = 0;

    /* sum over partitions with sizes a and n‑a, a ≤ n/2 and a ≥ 1 */
    idx_t a_max = (r-1)/2;
    for (idx_t a = 1; a <= a_max; ++a) {
        if (k > a*(r-a)) continue;  /* binomial -> 0 for k > a*(n-a) */
        seq_t term = binomial(r, a) * binomial(a*(r-a), k);
        sum += term;
    }

    /* special handling when the two color classes have equal size */
    if ((r & 1) == 0) {            /* r even */
        idx_t a_half = r/2;
        if (k <= a_half*a_half) {
            seq_t choose_half = binomial(r, a_half) / (seq_t)2;
            seq_t term = choose_half * binomial(a_half*a_half, k);
            sum += term;
        }
    }

    return sum;
}
