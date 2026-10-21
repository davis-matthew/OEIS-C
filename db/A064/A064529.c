/*
 * A064529  Number of connected components remaining when n-th letter of
 *          English alphabet is cut from a piece of paper.
 * https://oeis.org/A064529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 1 + number of closed loops in the uppercase letter with
 *            alphabetical index n (A=1, B=2, C=0, ...). The closed‑loop
 *            counts for A..Z are: 1,2,0,1,0,0,0,0,0,0,
 *            0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t closed_loops[26] = {
    1, /* A */
    2, /* B */
    0, /* C */
    1, /* D */
    0, /* E */
    0, /* F */
    0, /* G */
    0, /* H */
    0, /* I */
    0, /* J */
    0, /* K */
    0, /* L */
    0, /* M */
    0, /* N */
    1, /* O */
    1, /* P */
    1, /* Q */
    1, /* R */
    0, /* S */
    0, /* T */
    0, /* U */
    0, /* V */
    0, /* W */
    0, /* X */
    0, /* Y */
    0  /* Z */
};

seq_t a(idx_t n)
{
    /* The alphabet has 26 letters.  For n > 26 we wrap around. */
    idx_t idx = (n - 1) % 26;
    return closed_loops[idx] + 1;
}
