/*
 * A006684  Convolve Fibonacci and Pell numbers.
 * https://oeis.org/A006684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n) - Fibonacci(n), Pell(0)=0, Pell(1)=1,
 *            Pell(n)=2*Pell(n-1)+Pell(n-2); F(0)=0, F(1)=1,
 *            F(n)=F(n-1)+F(n-2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p0 = 0, p1 = 1;
    seq_t f0 = 0, f1 = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t pn = 2 * p1 + p0;
        seq_t fn = f1 + f0;
        p0 = p1;
        p1 = pn;
        f0 = f1;
        f1 = fn;
    }
    return p0 - f0;
}
