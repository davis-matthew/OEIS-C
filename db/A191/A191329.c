/*
 * A191329  (Lower Wythoff sequence mod 2)+(Upper Wythoff sequence mod 2).
 * https://oeis.org/A191329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor(n*phi) mod 2) + ((floor(n*phi) + n) mod 2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 9113, u64 n <= 9113,
 *            u128 n <= 9113, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute floor(n*phi) exactly using the identity
       floor(n*phi) = floor((n + floor(n*sqrt(5)))/2)                     */
    if (n == 0) return 0; /* undefined, but safe */

    seq_t N = (seq_t)n;
    seq_t v = (seq_t)5 * N * N;            // v = 5 * n^2
    seq_t sq = floor_sqrt(v);              // floor(sqrt(5 * n^2)) = floor(n*sqrt5)
    seq_t u = (N + sq) / 2;                // floor(n*phi)

    unsigned uParity = (unsigned)(u & 1);  /* parity of floor(n*phi)     */
    unsigned nParity = (unsigned)(N & 1);  /* parity of n                */

    /* a(n) = uParity + (uParity ^ nParity)   */
    seq_t result = (seq_t)(uParity + (uParity ^ nParity));
    return result;
}
