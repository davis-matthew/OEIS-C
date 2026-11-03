/*
 * A003474  Generalized Euler phi function (for p=3).
 * https://oeis.org/A003474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2 write n = 3^k*m with gcd(m,3) = 1, then
 *            a(n) = 3^(n-m) * Product_{d|m} (3^ord_d(3) -
 *            1)^(phi(d)/ord_d(3)), the number of units of GF(3)[x]/(x^n - 1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   ipow_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64.h"
#include "../utils/totient_u64.h"

static uint64_t a3474_ord3(uint64_t d) {
    uint64_t o = 1, pw;
    if (d == 1) return 1;
    pw = 3 % d;
    while (pw != 1) {
        pw = (pw * 3) % d;
        o++;
    }
    return o;
}

seq_t a(idx_t n) {
    uint64_t m, d;
    seq_t res;
    if (n == 1) return 1;
    m = (uint64_t)n;
    while (m % 3 == 0) m /= 3;
    res = ipow_u64((seq_t)3, (uint64_t)n - m);
    for (d = 1; d <= m; d++) {
        if (m % d == 0) {
            uint64_t o = a3474_ord3(d);
            uint64_t ph = totient_u64(d);
            seq_t f = ipow_u64((seq_t)3, o) - 1;
            res *= ipow_u64(f, ph / o);
        }
    }
    return res;
}
