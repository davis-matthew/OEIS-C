/*
 * A039649  a(n) = phi(n)+1.
 * https://oeis.org/A039649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(n) + 1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9720 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient.h"

seq_t a(idx_t n) {
    /* The offset is 1, so for n = 0 the function is undefined.
       Returning 0 for n == 0 keeps the implementation safe. */
    if (n == 0) return 0;
    return totient(n) + 1;
}
