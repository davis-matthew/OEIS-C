/*
 * A135295  a(n) = n^(number of decimal digits of n).
 * https://oeis.org/A135295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n ^ (number of decimal digits of n)
 * Range:     u8 n <= 15, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 51 (2026-11-01)
 * Helpers:   ipow, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/ipow.h"
#include "../utils/num_digits0.h"

seq_t a(idx_t n) {
    /* a(0) is defined as 0 */
    if (n == 0) return 0;

    /* Count decimal digits of n (n > 0). */
    uint64_t d = num_digits0(n);

    /* Compute n^d using the big_pow helper. */
    return ipow((seq_t)n, (seq_t)d);
}
