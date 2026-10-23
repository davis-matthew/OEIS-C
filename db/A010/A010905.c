/*
 * A010905  Pisot sequence E(4,15): a(n) = floor(a(n-1)^2/a(n-2)+1/2) for n>1,
 *          a(0)=4, a(1)=15.
 * https://oeis.org/A010905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) for n>=2, * a(0) = 4, a(1) = 15
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)15;
    seq_t a_nm2 = (seq_t)4;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)15;  /* a(n-1) */
    seq_t a_n;
    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)4 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
