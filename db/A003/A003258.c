/*
 * A003258  The number m such that c'(m) = A005206(A003231(n)), where c'(m) =
 *          A249115(m) is the m-th positive integer not in A003231.
 * https://oeis.org/A003258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m with A249115(m) = A005206(A003231(n)); computed as v =
 *            A005206(A003231(n)), a(n) = v - #{k >= 1 : A003231(k) <= v},
 *            where A003231(k) = floor(k*(5+sqrt(5))/2) and A005206(j) =
 *            floor((j+1)/phi)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   a3258_B, a3258_G, a3258_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3258_B.h"
#include "../utils/a3258_G.h"
#include "../utils/a3258_isqrt.h"

seq_t a(idx_t n) {
    uint64_t v = a3258_G(a3258_B((uint64_t)n));
    /* count k >= 1 with A003231(k) <= v; since A003231(k) < 4k, at least v/4 */
    uint64_t k = v / 4;
    while (a3258_B(k + 1) <= v) k++;
    return (seq_t)(v - k);
}
