/*
 * A115077  Number of 2 X 2 symmetric matrices over Z(n) having nonzero
 *          determinant.
 * https://oeis.org/A115077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n³ – B(n), where B(n) is the number of 2×2 symmetric
 *            matrices over ℤ/n with zero determinant. B(n) = ∏_{p^e‖n}
 *            f(p,e), and f(p,e) counts the solutions of ac ≡ b² (mod p^e) for
 *            all a,b,c∈ℤ/p^e
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   zero_det_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/zero_det_prime_power.h"

seq_t a(idx_t n)
{
    /* Compute zero‑determinant count B(n). */
    seq_t zero_count = (seq_t)1;
    idx_t tmp = n;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            zero_count *= zero_det_prime_power(p, exp);
        }
    }
    if (tmp > 1) { /* remaining prime factor */
        zero_count *= zero_det_prime_power(tmp, 1);
    }

    /* a(n) = n³ – B(n). */
    seq_t ns = (seq_t)n;
    seq_t n_cube = ns * ns * ns;
    return n_cube - zero_count;
}
