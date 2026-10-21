/*
 * A095802  Upper right triangular matrix T^2, where T(i,j) = (-1)^i*(1-2*i)
 *          for 1 <= i <= j.
 * https://oeis.org/A095802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = t_j * (S1(i) - S1(j-1)) where t_j = (-1)^j * (1-2j) and
 *            S1(m) = (-1)^{m+1} * m i is the row index satisfying i(i+1)/2 ≥
 *            n, j = n - i(i-1)/2
 * Range:     i8 n <= 27, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find row index i with i(i+1)/2 ≥ n */
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t sum = (__uint128_t)mid * (mid + 1) / 2;
        if (sum >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t i = lo;

    /* Compute column index j */
    __uint128_t prev = (__uint128_t)(i - 1) * i / 2;
    idx_t j = n - (idx_t)prev;          /* 1‑based column index */

    /* t_j = (-1)^j * (1-2j) */
    int parity_j = j & 1;
    seq_t sign_j = parity_j ? (seq_t)(-1) : (seq_t)1;
    seq_t tj = sign_j * ((seq_t)1 - (seq_t)2 * (seq_t)j);

    /* S1(i) = (-1)^{i+1} * i */
    int parity_i = i & 1;
    seq_t si = (parity_i ? (seq_t)1 : (seq_t)(-1)) * (seq_t)i;

    /* S1(j-1) = (-1)^j * (j-1) */
    seq_t sj = sign_j * (seq_t)(j - 1);

    seq_t delta = si - sj;
    return tj * delta;
}
