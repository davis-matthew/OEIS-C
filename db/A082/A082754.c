/*
 * A082754  Triangle read by rows: T(n, k) = abs(n^k-k^n), 1<=k<=n.
 * https://oeis.org/A082754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = abs(r^c – c^r) where r is the row (1‑based) containing
 *            term n in the triangle T(n,k)=|n^k−k^n|, and c is the column
 *            index in that row. The first term corresponds to r=1, c=1
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1)
            result *= cur;
        cur *= cur;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* safety guard, offset is 1 */
    /* Find row r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cum = 1;                  /* cumulative terms up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev = cum - r;           /* terms before this row: (r-1)*r/2 */
    idx_t c = n - prev;             /* column index within the row */

    seq_t r_pow = pow_seq((seq_t)r, c);
    seq_t c_pow = pow_seq((seq_t)c, r);

    if (r_pow >= c_pow)
        return r_pow - c_pow;
    else
        return c_pow - r_pow;
}
