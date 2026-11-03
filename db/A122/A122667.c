/*
 * A122667  a(n) = d(n)_d(n) = A122618(d(n)), where d = A000005, and A122618 =
 *          "n read in base n".
 * https://oeis.org/A122667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (the number “d” of divisors of n) read in base “d”, i.e.
 *            a(n) = A122618(d(n)) where d(n) = A000005(n)
 * Range:     u8 n <= 5039, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9666 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/num_divisors.h"

/* Convert a non‑negative integer m (fit in uint64_t) into the value
   obtained by interpreting its decimal digit string in base m.  The
   result is returned as type seq_t.  The function works for any seq_t
   that supports the required operations. */
static seq_t read_in_base_uint64(uint64_t m)
{
    seq_t result = 0;

    if (m == 0)
        return 0;                      /* base 0 is not used, return 0 */

    /* Extract decimal digits of m into a buffer (max 20 digits for
       64‑bit unsigned integers). */
    uint8_t d[20];
    size_t len = 0;
    uint64_t t = m;
    while (t > 0) {
        d[len++] = (uint8_t)(t % 10);
        t /= 10;
    }

    /* Process digits from most significant to least. */
    for (size_t i = 0; i < len; ++i) {
        result = result * (seq_t)m + (seq_t)d[len - 1 - i];
    }

    return result;
}

/* Required public function. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence.  Compute the divisor
       count d(n).  divisor_count_u64 is provided by the build. */
    uint64_t d = num_divisors((uint64_t)n);

    /* Treat d as the base and read its decimal digits in that base. */
    return read_in_base_uint64(d);
}
