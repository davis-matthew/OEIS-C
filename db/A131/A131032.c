/*
 * A131032  A097806 * A130296.
 * https://oeis.org/A131032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n ≥ 1, let r be the unique integer such that
 *            triangular(r-1)+1 ≤ n ≤ triangular(r) (triangular(k) =
 *            k(k+1)/2). Let c = n - triangular(r-1). Then a(n) = { 2*r-1 if c
 *            == 1 { 1 if c == r { 2 otherwise } where r and c are 1‑based row
 *            and column indices
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n<1, but safe guard */

    /* Find the row r such that triangular(r) >= n */
    idx_t r = 1;
    idx_t tri = 1;  /* triangular(r) */
    while (tri < n) {
        ++r;
        tri += r;  /* now tri == triangular(r) */
    }

    /* Compute column index c (1-based) */
    idx_t prev_tri = tri - r;          /* triangular(r-1) */
    idx_t c = n - prev_tri;            /* 1 ≤ c ≤ r */

    /* Compute the value according to the explicit rule */
    if (c == 1) {
        return ((seq_t)2 * r) - 1;     /* 2r-1 */
    } else if (c == r) {
        return 1;
    } else {
        return 2;
    }
}
