/*
 * A221441  Number of n X 3 binary arrays with every 0 a horizontal or
 *          antidiagonal neighbor to some 1.
 * https://oeis.org/A221441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 11*a(n-2) + a(n-3), a(1)=5, a(2)=33, a(3)=221
 *            (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)33;
    if (n == 3) return (seq_t)221;

    seq_t a_nm3 = (seq_t)5;    /* a(n-3)   */
    seq_t a_nm2 = (seq_t)33;   /* a(n-2)   */
    seq_t a_nm1 = (seq_t)221;  /* a(n-1)   */
    seq_t a_n;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)5 * a_nm1 + (seq_t)11 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
