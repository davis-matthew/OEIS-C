/*
 * A221131  Table, T, read by antidiagonals where T(-j,k) = ((1+sqrt(j))^k +
 *          (1-sqrt(j))^k)/2.
 * https://oeis.org/A221131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(-r,k) where r,k are derived from n by antidiagonal
 *            order: Let s satisfy s*(s+1)/2 <= n < (s+1)*(s+2)/2. Let t = n -
 *            s*(s+1)/2. Then r = s - t, k = t. Recurrence for each fixed r:
 *            T(-r,0)=T(-r,1)=1, T(-r,i+2)=2*T(-r,i+1) - (1+r)*T(-r,i)
 * Range:     i8 n <= 71, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Determine the diagonal s containing index n */
    idx_t s = 0;
    while (( (__uint128_t)(s+1) * (s+2) ) / 2 <= n) {
        ++s;
    }
    /* Cumulative count before this diagonal */
    idx_t before = (idx_t)(( (__uint128_t)s * (s+1) ) / 2);
    idx_t t = n - before;          /* offset within the diagonal   */
    idx_t r = s - t;               /* row index (non‑negative)     */
    idx_t k = t;                   /* column index                 */

    /* Compute T(-r,k) using the recurrence */
    if (k == 0 || k == 1) {
        return (seq_t)1;
    }
    seq_t t0 = 1;          /* T(-r,0) */
    seq_t t1 = 1;          /* T(-r,1) */
    seq_t coeff = (seq_t)(1 + r);  /* (1 + r) = 1 - j where j = -r */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t t2 = 2 * t1 - coeff * t0;
        t0 = t1;
        t1 = t2;
    }
    return t1;
}
