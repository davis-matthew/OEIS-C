/*
 * A213821  Rectangular array: (row n) = b**c, where b(h) = 3*h-1, c(h) =
 *          n-1+h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A213821: Rectangular array T(n,k) = 4*T(n,k-1)-6*T(n,k-2)+4*T(n,k-3)-T(n,k-4)
    with initial values for a given row n:
        T(n,1) = 2*n
        T(n,2) = 7*n + 2
        T(n,3) = 15*n + 9
        T(n,4) = 26*n + 24
    The sequence a(n) is obtained by reading the array in falling
    antidiagonals: a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), etc.
*/

#include <stdint.h>

static seq_t T_val(idx_t row, idx_t col) {
    /* Compute T(row, col) using the recurrence and initial values */
    seq_t r = (seq_t)row;
    if (col == 1) return 2 * r;
    if (col == 2) return 7 * r + 2;
    if (col == 3) return 15 * r + 9;
    if (col == 4) return 26 * r + 24;

    /* Iterate using the order‑4 recurrence */
    seq_t t1 = 2 * r;          /* k = 1   */
    seq_t t2 = 7 * r + 2;      /* k = 2   */
    seq_t t3 = 15 * r + 9;     /* k = 3   */
    seq_t t4 = 26 * r + 24;    /* k = 4   */
    seq_t cur;
    for (idx_t k = 5; k <= col; ++k) {
        cur = 4 * t4 - 6 * t3 + 4 * t2 - t1;
        t1 = t2; t2 = t3; t3 = t4; t4 = cur;
    }
    return t4;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but guard against misuse */

    /* Find sum s such that C(s,2) >= n */
    idx_t s = (idx_t)((__builtin_sqrt((double)(1 + 8 * (long double)n)) + 1) / 2);
    /* Adjust a few steps to get exact s */
    while ((unsigned __int128)s * (s - 1) / 2 < n) ++s;
    while ((unsigned __int128)(s - 1) * (s - 2) / 2 >= n) --s;

    idx_t prev = (s > 1) ? (unsigned __int128)(s - 1) * (s - 2) / 2 : 0;
    idx_t r = n - prev;          /* 1 ≤ r ≤ s-1 */
    idx_t i = r;                 /* row index   */
    idx_t j = s - r;             /* column index */

    return T_val(i, j);
}
