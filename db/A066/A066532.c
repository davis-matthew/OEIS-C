/*
 * A066532  If n is odd a(n) = 1, if n is even a(n) = 2^(n-1).
 * https://oeis.org/A066532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is odd) ? 1 : 2^(n-1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n & 1U) {                   /* odd n */
        return (seq_t)1;
    }
    seq_t result = (seq_t)1;
    /* shift left by n-1; safe for arbitrary‑precision types,
       for fixed width it will wrap if the value does not fit, which
       is acceptable because the test only checks terms that fit in the
       target type. */
    result <<= (unsigned)(n - 1);
    return result;
}
