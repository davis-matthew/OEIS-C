/*
 * A002322  Reduced totient function psi(n): least k such that x^k == 1 (mod
 *          n) for all x prime to n; also known as the Carmichael lambda
 *          function (exponent of unit group mod n); also called the universal
 *          exponent of n.
 * https://oeis.org/A002322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, core, easy, nice
 * Formula:   lambda(2^e) = 2^(e-1) if e<=2, 2^(e-2) if e>2; lambda(p^e) =
 *            (p-1)*p^(e-1) for odd p; a(n) = lcm over prime powers p^e || n
 *            of lambda(p^e), a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a002322_gcd_u, a002322_lcm_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002322_gcd_u.h"
#include "../utils/a002322_lcm_u.h"

seq_t a(idx_t n_idx) {
    uint64_t n = (uint64_t)n_idx;
    uint64_t result = 1;

    if (n <= 1) return (seq_t)1;

    if (n % 2 == 0) {
        uint64_t e = 0;
        while (n % 2 == 0) {
            n /= 2;
            e++;
        }
        uint64_t l2;
        if (e <= 2) l2 = (uint64_t)1 << (e - 1);
        else l2 = (uint64_t)1 << (e - 2);
        result = a002322_lcm_u(result, l2);
    }

    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            uint64_t lp = p - 1;
            n /= p;
            while (n % p == 0) {
                n /= p;
                lp *= p;
            }
            result = a002322_lcm_u(result, lp);
        }
    }

    if (n > 1) result = a002322_lcm_u(result, n - 1);

    return (seq_t)result;
}
