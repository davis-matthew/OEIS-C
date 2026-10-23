/*
 * A045663  Number of 2n-bead balanced binary strings of fundamental period
 *          2n, rotationally equivalent to complement.
 * https://oeis.org/A045663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n == 0 a(n) = Sum_{d|n, d odd} mu(d) * 2 ^ (n/d) for
 *            n > 0 where mu is the Möbius function
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   mobius_int, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"     /* provides seq_t pow2(idx_t); */
#include "../utils/mobius_int.h"

seq_t a(idx_t n)
{
    if (n == 0u) return 1;

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;

    for (idx_t d = 1u; d * d <= n; ++d) {
        if (n % d == 0u) {
            if (d & 1u) {
                int mu = mobius_int(d);
                seq_t val = pow2(n / d);
                if (mu > 0) sum_pos += val;
                else if (mu < 0) sum_neg += val;
            }
            idx_t d2 = n / d;
            if (d2 != d && (d2 & 1u)) {
                int mu = mobius_int(d2);
                seq_t val = pow2(n / d2);
                if (mu > 0) sum_pos += val;
                else if (mu < 0) sum_neg += val;
            }
        }
    }
    /* Since a(n) >= 0, sum_pos >= sum_neg.  Unsigned subtraction is safe. */
    return sum_pos - sum_neg;
}
