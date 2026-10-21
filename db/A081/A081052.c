/*
 * A081052  Difference of Stirling numbers of the first kind.
 * https://oeis.org/A081052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = s(n,2) - s(n,3), where s(n,m) is the signed Stirling
 *            number of the first kind. Recurrence (for n ≥ 3): a(n) =
 *            -3*(n-1)*a(n-1) - (3*n^2-9*n+7)*a(n-2) - (n-2)^3*a(n-3) with
 *            initial values a(0)=0, a(1)=1, a(2)=-4
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
seq_t a(idx_t n) {
    /* Handle the known initial values */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-4;
 
    /* Keep the last three computed terms */
    seq_t a_m3 = (seq_t)0;   /* a(n-3) */
    seq_t a_m2 = (seq_t)1;   /* a(n-2) */
    seq_t a_m1 = (seq_t)-4;  /* a(n-1) */
    seq_t a_cur = a_m1;      /* placeholder for a(n) */
 
    for (idx_t i = 3; i <= n; ++i) {
        seq_t term1 = (seq_t)3 * (seq_t)(i-1) * a_m1;
        seq_t coeff2 = (seq_t)(3*i*i - 9*i + 7);
        seq_t term2 = coeff2 * a_m2;
        seq_t term3 = (seq_t)(i-2)*(i-2)*(i-2) * a_m3;
        a_cur = -term1 - term2 - term3;
 
        /* Shift the window of last three terms */
        a_m3 = a_m2;
        a_m2 = a_m1;
        a_m1 = a_cur;
    }
    return a_cur;
}
