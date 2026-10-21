/*
 * A083133  Number the letters of English alphabet from 1 to 26; sequence
 *          specifies capital letters with no symmetry.
 * https://oeis.org/A083133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn, fini, full
 * Formula:   a(n) = indices of English capital letters that have no symmetry
 *            (vertical, horizontal, or rotational). The unsymmetric letters
 *            are F(6), G(7), J(10), L(12), P(16), Q(17), R(18)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const bool unsym[27] = {
    /* 0 */ 0,
    /* 1 A */ 0,
    /* 2 B */ 0,
    /* 3 C */ 0,
    /* 4 D */ 0,
    /* 5 E */ 0,
    /* 6 F */ 1,
    /* 7 G */ 1,
    /* 8 H */ 0,
    /* 9 I */ 0,
    /*10 J */ 1,
    /*11 K */ 0,
    /*12 L */ 1,
    /*13 M */ 0,
    /*14 N */ 0,
    /*15 O */ 0,
    /*16 P */ 1,
    /*17 Q */ 1,
    /*18 R */ 1,
    /*19 S */ 0,
    /*20 T */ 0,
    /*21 U */ 0,
    /*22 V */ 0,
    /*23 W */ 0,
    /*24 X */ 0,
    /*25 Y */ 0,
    /*26 Z */ 0
};

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t i = 1; i <= 26; ++i) {
        if (unsym[i]) {
            ++count;
            if (count == n) return (seq_t)i;
        }
    }
    return 0; /* undefined for n beyond the list */
}
