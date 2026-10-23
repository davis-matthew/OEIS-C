/*
 * A104618  Write the natural numbers in base 15 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   digit_at_pos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the digit that appears at position n(n+1)/2 in the stream obtained
   by concatenating the base‑15 representations of the natural numbers
   (starting with 1). */

#include <stdint.h>
#include "../utils/digit_at_pos.h"

/* External linkage function required by the OEIS build */
seq_t a(idx_t n) {
    /* Position of the last digit of row n */
    seq_t pos = ((seq_t)n * ((seq_t)n + 1)) / 2;
    return digit_at_pos(pos);
}
