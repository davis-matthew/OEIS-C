/*
 * A003270  A nonrepetitive sequence.
 * https://oeis.org/A003270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Arshon's square-free word, the fixed point starting with 1 of
 *            the map where the letter k at an odd position becomes k,k+1,k+2
 *            and at an even position becomes k+2,k+1,k (letters taken mod 3,
 *            in 1..3). With n-1 = 3q + r (0 <= r < 3) and c = a(q+1): a(n) =
 *            ((c-1) + r) mod 3 + 1 if q+1 is odd, a(n) =...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   arshon
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/arshon.h"

seq_t a(idx_t n)
{
    return (seq_t)arshon(n);
}
