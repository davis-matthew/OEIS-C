/*
 * A128542  a(n) = ((2n)^(2n) - 1)/((2n+1)*(2n-1)).
 * https://oeis.org/A128542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2n)^(2n)-1)/((2n+1)*(2n-1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 188 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    /* Handle the offset 0 explicitly to avoid underflow in the denominator. */
    if (n == 0) {
        return 0;
    }

    seq_t p = (seq_t)(2 * n);          /* p = 2n  */
    seq_t exp = p;                     /* exp = 2n */

    /* Compute numerator: p^exp - 1 */
    seq_t num = ipow(p, exp) - 1;

    /* Compute denominator: (p+1)*(p-1) = (2n+1)*(2n-1) */
    seq_t denom = ((p + 1) * (p - 1));

    return num / denom;
}
