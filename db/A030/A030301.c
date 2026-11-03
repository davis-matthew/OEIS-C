/*
 * A030301  n-th run has length 2^(n-1).
 * https://oeis.org/A030301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 if floor(log2(n)) is odd, else 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9296 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is assumed to be >= 1 (offset 1). */
    idx_t k = 0;
    idx_t m = n;
    while (m >>= 1) {
        ++k;
    }
    return (seq_t)(k & 1);
}
