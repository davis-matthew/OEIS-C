/*
 * A001795  Coefficient of P_0(x) in the Legendre expansion of x^n.
 * https://oeis.org/A001795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = numerator(A000108(2*n)/4^n) = A000108(2*n)/2^A000120(n);
 *            computed by a(n) = a(n-1)*(4n-1)*(4n-3)/(o(n)*(2n+1)), a(0) = 1,
 *            where o(n) is the odd part of n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   a1795_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1795_g.h"

seq_t a(idx_t n) {
    seq_t x = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        idx_t o = k;
        idx_t d2 = 2 * k + 1;
        idx_t p1 = 4 * k - 1;
        idx_t p2 = 4 * k - 3;
        idx_t g;
        while ((o & 1) == 0) o >>= 1;
        g = a1795_g(p1, o); p1 /= g; o /= g;
        g = a1795_g(p2, o); p2 /= g; o /= g;
        g = a1795_g(p1, d2); p1 /= g; d2 /= g;
        g = a1795_g(p2, d2); p2 /= g; d2 /= g;
        if (o > 1) x /= o;
        if (d2 > 1) x /= d2;
        x *= p1;
        x *= p2;
    }
    return x;
}
