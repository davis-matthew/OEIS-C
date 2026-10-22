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
 * Helpers:   a3474_ord3, a3474_pow, a3474_totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3474_ord3.h"
#include "../utils/a3474_pow.h"
#include "../utils/a3474_totient.h"

seq_t a(idx_t n) {
    uint64_t m, d;
    seq_t res;
    if (n == 1) return 1;
    m = (uint64_t)n;
    while (m % 3 == 0) m /= 3;
    res = a3474_pow((seq_t)3, (uint64_t)n - m);
    for (d = 1; d <= m; d++) {
        if (m % d == 0) {
            uint64_t o = a3474_ord3(d);
            uint64_t ph = a3474_totient(d);
            seq_t f = a3474_pow((seq_t)3, o) - 1;
            res *= a3474_pow(f, ph / o);
        }
    }
    return res;
}
