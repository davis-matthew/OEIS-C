/*
 * A000778  a(n) = Catalan(n) + Catalan(n+1) - 1.
 * https://oeis.org/A000778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) + Catalan(n+1) - 1, using Catalan(0) = 1 and
 *            Catalan(k+1) = Catalan(k)*(4k+2)/(k+2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a778_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a778_gcd.h"

seq_t a(idx_t n) {
    seq_t c = 1;      /* Catalan(k) */
    seq_t cn = 1;     /* Catalan(n) once reached */
    idx_t k;
    for (k = 0; k <= n; k++) {
        idx_t num = 4 * k + 2;
        idx_t den = k + 2;
        idx_t g = a778_gcd(num, den);
        num /= g;
        den /= g;
        if (k == n) cn = c;
        /* den is coprime to num, so den divides Catalan(k) */
        c = (c / den) * num;
    }
    /* c now holds Catalan(n+1) */
    return cn + c - 1;
}
