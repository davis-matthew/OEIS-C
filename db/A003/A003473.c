/*
 * A003473  Generalized Euler phi function (for p=2).
 * https://oeis.org/A003473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with n = 2^k*m, m odd: a(n) = 2^(n-m) * Product_{d|m}
 *            (2^ord_d(2) - 1)^(phi(d)/ord_d(2)), i.e. number of units of
 *            GF(2)[x]/(x^n-1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   a3473_ord2, a3473_phi, a3473_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3473_ord2.h"
#include "../utils/a3473_phi.h"
#include "../utils/a3473_pw.h"

seq_t a(idx_t n) {
    uint64_t m = n, d, i;
    seq_t result = 1;
    while (m % 2 == 0) m /= 2;
    for (i = 0; i < n - m; i++) result *= 2;
    for (d = 3; d <= m; d += 2) {
        if (m % d == 0) {
            uint64_t r = a3473_ord2(d);
            uint64_t ph = a3473_phi(d);
            seq_t q = 1;
            for (i = 0; i < r; i++) q *= 2;
            q -= 1;
            result *= a3473_pw(q, ph / r);
        }
    }
    return result;
}
