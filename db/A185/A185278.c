/*
 * A185278  Number of isomorphism classes of generalized Petersen graphs
 *          G(n,k) on 2n vertices with gcd(n,k) = 1.
 * https://oeis.org/A185278
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (phi(n) + A060594(n) + A000089(n)) / 4 * where A060594(n)
 *            = number of solutions to x^2 ≡ 1 (mod n) * and A000089(n) =
 *            number of solutions to x^2 ≡ -1 (mod n)
 * Range:     u8 n <= 897, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _solutions_x2eq1, _solutions_x2eqm1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Value type aliases (supplied by the build) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_solutions_x2eq1.h"
#include "../utils/_solutions_x2eqm1.h"

/* Small helper: compute phi(n) efficiently */
static seq_t _phi_uint64(uint64_t n) {
    uint64_t result = n;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            result -= result / p;
        }
    }
    if (tmp > 1) result -= result / tmp;
    return (seq_t)result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 3) return (seq_t)0;   /* undefined for n < 3 */

    uint64_t n64 = (uint64_t)n;

    seq_t phi_val = _phi_uint64(n64);
    seq_t B = _solutions_x2eq1(n64);
    seq_t C = _solutions_x2eqm1(n64);

    return (phi_val + B + C) / 4;
}
