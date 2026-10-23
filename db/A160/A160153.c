/*
 * A160153  Numerator of Hermite(n, 26/27).
 * https://oeis.org/A160153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 27^n * Hermite(n, 26/27)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=52, a(n+1)=52*a(n)-1458*n*a(n-1) */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)52;
    /* a(n-2) and a(n-1) will hold previous two values */
    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)52;  /* a(1) */
    seq_t res;
    for (idx_t i = 1; i < n; ++i) {          /* i corresponds to k = n-1 */
        seq_t coeff = (seq_t)(1458 * i);      /* 1458 * (n-1) */
        /* res = 52 * a_nm1 - coeff * a_nm2 */
        res = ((seq_t)52) * a_nm1 - coeff * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = res;
    }
    return a_nm1;
}
