/*
 * A003821  a(1)=a(2)=1, a(n+1) = (a(n)^6 +1)/a(n-1).
 * https://oeis.org/A003821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (a(n)^6 + 1)/a(n-1), a(1) = a(2) = 1
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p = 1, q = 1, r, q2, q3;
    idx_t i;
    if (n <= 2) return 1;
    for (i = 3; i <= n; i++) {
        q2 = q * q;
        q3 = q2 * q;
        r = q3 * q3 + 1;
        if (p == 0) return 0; /* only reachable after fixed-width overflow */
        r = r / p;
        p = q;
        q = r;
    }
    return q;
}
