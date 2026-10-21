/*
 * A074353  Coefficient of q^2 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(1,2).
 * https://oeis.org/A074353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+3*a(n-2)-11*a(n-3)-6*a(n-4)+12*a(n-5)+8*a(n-6)
 *            for n>7 with initial terms
 *            a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=6,a(5)=20,a(6)=70,a(7)=196
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 57, u128 n <= 118,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* small indices: direct table */
    if (n < 8) {
        static const seq_t base[8] = {0, 0, 0, 0, 6, 20, 70, 196};
        return base[n];
    }

    /* rolling buffer of the last 8 values */
    seq_t buf[8] = {0, 0, 0, 0, 6, 20, 70, 196};

    for (idx_t i = 8; i <= n; ++i) {
        seq_t sum = 0;
        sum = sum + mul_small(buf[(i - 1) & 7], 3);
        sum = sum + mul_small(buf[(i - 2) & 7], 3);
        sum = sum + mul_small(buf[(i - 5) & 7], 12);
        sum = sum + mul_small(buf[(i - 6) & 7], 8);

        seq_t sub = 0;
        sub = sub + mul_small(buf[(i - 3) & 7], 11);
        sub = sub + mul_small(buf[(i - 4) & 7], 6);

        buf[i & 7] = sum - sub;  /* sum >= sub by construction */
    }

    return buf[n & 7];
}
