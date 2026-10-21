/*
 * A238452  Second column of the extended Catalan triangle A189231.
 * https://oeis.org/A238452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * binomial(n, floor(n/2)+1) for even n a(n) =
 *            binomial(n+1, floor(n/2)+1) / (floor(n/2)+2) for odd n a(0) = 0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if ((n & 1ULL) == 0) {            /* n is even */
        idx_t k = n / 2 + 1;          /* floor(n/2)+1 */
        seq_t bin = binomial(n, k);   /* binomial(n, k) */
        return 2 * bin;
    } else {                          /* n is odd */
        idx_t k = n / 2 + 1;          /* floor(n/2)+1 */
        seq_t bin = binomial(n + 1, k); /* binomial(n+1, k) */
        seq_t denom = k + 1;          /* floor(n/2)+2 */
        return bin / denom;
    }
}
