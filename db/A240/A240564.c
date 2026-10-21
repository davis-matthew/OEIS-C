/*
 * A240564  A number (where A=1, B=2...) for every letter in the capitalized
 *          alphabet that does not have a curved line in the letter.
 * https://oeis.org/A240564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A240564: the position of the nth capital letter that has no curved line
   by scanning the alphabet. */
#include <stdint.h>

static const uint8_t no_curved[26] = {
    1, /* A */
    0, /* B */
    0, /* C */
    0, /* D */
    1, /* E */
    1, /* F */
    0, /* G */
    1, /* H */
    1, /* I */
    0, /* J */
    1, /* K */
    1, /* L */
    1, /* M */
    1, /* N */
    0, /* O */
    0, /* P */
    0, /* Q */
    0, /* R */
    0, /* S */
    1, /* T */
    0, /* U */
    1, /* V */
    1, /* W */
    1, /* X */
    1, /* Y */
    1  /* Z */
};

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t i = 1; i <= 26; ++i) {
        if (no_curved[i - 1]) {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
    return 0; /* n larger than number of such letters */
}
