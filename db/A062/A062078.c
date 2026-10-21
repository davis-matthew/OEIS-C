/*
 * A062078  LCM of the digits of n.
 * https://oeis.org/A062078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = LCM of the decimal digits of n; if any digit is 0, the
 *            result is 0
 * Range:     u8 n <= 577, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gcd_seq_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation is O(number of digits of n) per call. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/_gcd_seq_local.h"

seq_t a(idx_t n)
{
    /* Any digit 0 makes the LCM 0 */
    if (n == 0) return 0;   /* safety for n==0, though offset starts at 1 */

    seq_t lcm = 1;
    while (n > 0) {
        unsigned d = (unsigned)(n % 10);
        if (d == 0) return 0;
        seq_t g = _gcd_seq_local(lcm, (seq_t)d);
        lcm = (lcm / g) * d;
        n /= 10;
    }
    return lcm;
}
