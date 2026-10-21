/*
 * A064530  Number of holes in n-th capital letter of English alphabet.
 * https://oeis.org/A064530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of closed loops in the capital letter at position n
 * in the English alphabet.  The sequence repeats every 26 letters.
 *
 * Letter holes:
 *   A(1)  – 1
 *   B(2)  – 2
 *   C(3)  – 0
 *   D(4)  – 1
 *   E(5)  – 0
 *   F(6)  – 0
 *   G(7)  – 0
 *   H(8)  – 0
 *   I(9)  – 0
 *   J(10) – 0
 *   K(11) – 0
 *   L(12) – 0
 *   M(13) – 0
 *   N(14) – 0
 *   O(15) – 1
 *   P(16) – 1
 *   Q(17) – 1
 *   R(18) – 1
 *   S(19) – 0
 *   T(20) – 0
 *   U(21) – 0
 *   V(22) – 0
 *   W(23) – 0
 *   X(24) – 0
 *   Y(25) – 0
 *   Z(26) – 0
 *
 * For n > 26 the alphabet cycles, so a(n) = a(((n-1) % 26) + 1).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Map n to a letter index in [1,26] */
    idx_t idx = (n - 1) % 26 + 1;

    /* Determine holes for that letter */
    switch (idx) {
        case 1:  /* A */
        case 4:  /* D */
        case 15: /* O */
        case 16: /* P */
        case 17: /* Q */
        case 18: /* R */
            return (seq_t)1;
        case 2:  /* B */
            return (seq_t)2;
        default: /* all others have 0 holes */
            return (seq_t)0;
    }
}
