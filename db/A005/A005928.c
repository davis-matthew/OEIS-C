/*
 * A005928  G.f.: s(1)^3/s(3), where s(k) = eta(q^k) and eta(q) is Dedekind's
 *          function, cf. A010815.
 * https://oeis.org/A005928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -3 * A123477(n) for n > 0, a(0) = 1, where A123477 is
 *            multiplicative with: A123477(p^e) = -2 if p = 3 and e > 0
 *            A123477(p^e) = e+1 if p ≡ 1 (mod 6) A123477(p^e) = (1 +
 *            (-1)^e)/2 if p ≡ 2, 5 (mod 6)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   compute_A123477
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_A123477.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return -3 * compute_A123477(n);
}
