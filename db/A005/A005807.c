/*
 * A005807  Sum of adjacent Catalan numbers.
 * https://oeis.org/A005807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n) + C(n+1), with Catalan C(0) = 1, C(k+1) =
 *            C(k)*2*(2k+1)/(k+2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a005807_gcd64, a005807_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005807_gcd64.h"
#include "../utils/a005807_next.h"

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 0; k < n; k++)
        c = a005807_next(c, k);
    return c + a005807_next(c, n);
}
