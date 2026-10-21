/*
 * A202834  E.g.f.: exp(3*x + x^2/2).
 * https://oeis.org/A202834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + (n-1)*a(n-2), a(0)=1, a(1)=3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a_nm2 = (seq_t)1;      /* a(0) */
    seq_t a_nm1 = (seq_t)3;      /* a(1) */
    seq_t a_n   = 0;
    for (idx_t i = 2; i <= n; ++i) {
        a_n = 3 * a_nm1 + (seq_t)(i - 1) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
