/*
 * A000316  Two decks each have n kinds of cards, 2 of each kind. The first
 *          deck is laid out in order. The second deck is shuffled and laid
 *          out next to the first. A match occurs if a card from the second
 *          deck is next to a card of the same kind from the first deck. a(n)
 *          is the number of ways of achieving no m...
 * https://oeis.org/A000316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   (2*n-3)*a(n) = 2*(n-1)*(2*n-1)^2*a(n-1) + 4*(n-1)*(2*n-3)*a(n-2)
 *            - 16*(n-2)*(n-1)*(2*n-1)*a(n-3), a(0) = 1, a(1) = 0, a(2) = 4
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   a316_big, a316_fixed, a316_inv, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/a316_big.h"
#include "../utils/a316_fixed.h"
#include "../utils/a316_inv.h"

seq_t a(idx_t n)
{
    if (seq_is_big())
        return a316_big(n);
    return a316_fixed(n);
}
