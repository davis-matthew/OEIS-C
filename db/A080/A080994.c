/*
 * A080994  Number of letters and blanks in Scrabble (English version).
 * https://oeis.org/A080994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = number of Scrabble tiles representing the nth letter of
 *            the alphabet (A=1, B=2, …, Z=26) plus the two blank tiles at
 *            position 27. For n > 27 the sequence is undefined, so we return
 *            0
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Scrabble tile distribution for the English set */
    static const seq_t tbl[27] = {
        9, /* A */
        2, /* B */
        2, /* C */
        4, /* D */
        12,/* E */
        2, /* F */
        3, /* G */
        2, /* H */
        9, /* I */
        1, /* J */
        1, /* K */
        4, /* L */
        2, /* M */
        6, /* N */
        8, /* O */
        2, /* P */
        1, /* Q */
        6, /* R */
        4, /* S */
        6, /* T */
        4, /* U */
        2, /* V */
        2, /* W */
        1, /* X */
        2, /* Y */
        1, /* Z */
        2  /* blanks */
    };

    if (n == 0) return 0;               /* offset = 1, n should start at 1 */
    if (n <= 27) return tbl[n - 1];     /* valid index */
    return 0;                           /* undefined extension */
}
