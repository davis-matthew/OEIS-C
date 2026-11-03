/*
 * A122666  a(n) = n_d(n) where d() = A000005.
 * https://oeis.org/A122666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = value obtained by interpreting the decimal digits of n as
 *            a base‑d(n) number, where d(n) is the number of divisors of n
 *            (A000005)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1, 0 is unused */

    seq_t base = (seq_t)num_divisors((uint64_t)n);  /* d(n) */
    seq_t result = 0;
    seq_t power  = 1;
    idx_t t = n;

    while (t > 0) {
        unsigned digit = t % 10;          /* little‑end digit in base 10 */
        result += (seq_t)digit * power;   /* accumulate digit * base^position */
        power *= base;                    /* next positional weight */
        t /= 10;
    }
    return result;
}
