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
 * Helpers:   divisor_count_u64, read_in_base_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/divisor_count_u64.h"
#include "../utils/read_in_base_uint64.h"

/* Required public function. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence.  Compute the divisor
       count d(n).  divisor_count_u64 is provided by the build. */
    uint64_t d = divisor_count_u64((uint64_t)n);

    /* Treat d as the base and read its decimal digits in that base. */
    return read_in_base_uint64(d);
}
