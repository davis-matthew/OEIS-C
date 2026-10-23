/*
 * A120065  Number of permutations on 1..n where gcd(s_i,n) = gcd(i,n). Also
 *          Product_{d divides n} phi(d)!.
 * https://oeis.org/A120065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{d|n} φ(d)!
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 25,
 *            big any n
 * Helpers:   factorial, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for index 0, return 0 */

    seq_t product = 1;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            seq_t phi_d = phi(d);
            product *= factorial((idx_t)phi_d);

            idx_t e = n / d;
            if (e != d) {
                seq_t phi_e = phi(e);
                product *= factorial((idx_t)phi_e);
            }
        }
    }

    return product;
}
