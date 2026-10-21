/*
 * A083134  Number the letters of English alphabet from 1 to 26; sequence
 *          specifies capital letters which are indistinguishable after
 *          180-degree rotation.
 * https://oeis.org/A083134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, fini, full
 * Formula:   a(n) = the nth letter number (1‑based) of the English alphabet
 *            that looks indistinguishable after a 180° rotation. The eligible
 *            letters are H(8), I(9), N(14), O(15), S(19), X(24), Z(26)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Bit mask of 26 letters: bit i is set if letter (i+1) is eligible. */
    const uint64_t mask = (1ULL << (8 - 1))  /* H */
                        | (1ULL << (9 - 1))  /* I */
                        | (1ULL << (14 - 1)) /* N */
                        | (1ULL << (15 - 1)) /* O */
                        | (1ULL << (19 - 1)) /* S */
                        | (1ULL << (24 - 1)) /* X */
                        | (1ULL << (26 - 1));/* Z */

    if (n == 0) return 0;                 /* offset 1 */
    idx_t cnt = 0;
    for (idx_t i = 1; i <= 26; ++i) {
        if (mask & (1ULL << (i - 1))) {
            ++cnt;
            if (cnt == n) {
                return (seq_t)i;
            }
        }
    }
    /* n is larger than the number of eligible letters (7) */
    return 0;
}
