/*
 * A038190  Pagoda sequence: a(0) = b(n)-b(n-2) mod 3, where b(n) =
 *          A038189(n).
 * https://oeis.org/A038190
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the (n+2)-th digit of the Pagoda word. The word is
 *            obtained by iterating the morphism A→AB, B→AD, C→CB, D→CD, and
 *            then mapping the letters to quads A→2201, B→0211, C→0221,
 *            D→1201. The digit values are 0,1,2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   _letter_at
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_letter_at.h"

static const int _digit_map[4][4] = {
    {2, 2, 0, 1},   /* A */
    {0, 2, 1, 1},   /* B */
    {0, 2, 2, 1},   /* C */
    {1, 2, 0, 1}    /* D */
};

/* Return the digit of the Pagoda word at position pos (pos ≥ 0). */
static seq_t _b(idx_t pos) {
    idx_t word_index = pos >> 2;   /* divide by 4 */
    int offset = pos & 3;          /* modulo 4 */
    int letter = _letter_at(word_index);
    return (seq_t)_digit_map[letter][offset];
}

seq_t a(idx_t n) {
    /* a(n) = b(n+2) for all n ≥ –2 (the definition in OEIS). */
    if (n < -2) return 0;          /* undefined; return 0 just in case */
    idx_t k = n + 2;
    return _b(k);
}
