/*
 * A000150  Number of dissections of an n-gon, rooted at an exterior edge,
 *          asymmetric with respect to that edge.
 * https://oeis.org/A000150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(A000108(n)/2) - [n odd]*floor(A000108((n-1)/2)/2),
 *            i.e. a(2n) = A000108(2n)/2, a(2n+1) = (A000108(2n+1) -
 *            A000108(n))/2. Catalan numbers via C(k+1) = C(k)*2*(2k+1)/(k+2),
 *            with the small factors reduced by their gcd first so that no
 *            intermediate exceeds the result
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   catalan, half_catalan, small_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/half_catalan.h"
#include "../utils/small_g.h"

seq_t a(idx_t n)
{
    seq_t h = half_catalan(n);
    if (n & 1) {
        seq_t c = catalan((n - 1) / 2);
        h -= c >> 1;
    }
    return h;
}
