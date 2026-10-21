/*
 * A077568  a(1,n) as defined in A003148.
 * https://oeis.org/A077568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) + 2*(2*n+1)*(n-1)*a(n-2), a(0)=1, a(1)=-1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-1;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev  = (seq_t)-1;  /* a(1) */
    seq_t a_curr  = a_prev;     /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        /* factor = 2 * (2*i + 1) * (i-1) */
        seq_t factor = (seq_t)2 * ((seq_t)(2*i + 1)) * ((seq_t)(i - 1));
        a_curr = -a_prev + factor * a_prev2;
        a_prev2 = a_prev;
        a_prev  = a_curr;
    }
    return a_curr;
}
