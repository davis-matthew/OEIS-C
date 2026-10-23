/*
 * A188649  Least common multiple of reversals of divisors of n in decimal
 *          representation.
 * https://oeis.org/A188649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = LCM_{d|n} reverse_decimal(d)
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 209, u64 n <= 713,
 *            u128 n <= 5095, big any n
 * Helpers:   gcd, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the least common multiple of the decimal reversals of all
   positive divisors of n. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* outside the defined offset (starts at 1) */

    seq_t lcm = 1;
    idx_t d;

    for (d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t rev_d = reverse_decimal((seq_t)d);
            seq_t g = gcd(lcm, rev_d);
            lcm = (lcm / g) * rev_d;

            if (d != n / d) {
                seq_t rev_q = reverse_decimal((seq_t)(n / d));
                g = gcd(lcm, rev_q);
                lcm = (lcm / g) * rev_q;
            }
        }
    }
    return lcm;
}
