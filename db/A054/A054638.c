/*
 * A054638  0 if pronounced name of n-th letter of English alphabet begin with
 *          a vowel sound, otherwise 1. Different from A074322.
 * https://oeis.org/A054638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) = 0 if the pronounced name of the n‑th letter of the
 *            English alphabet begins with a vowel sound, otherwise 1. The
 *            sequence is defined for n = 1..26
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Values for the 26 English letters */
    static const seq_t vals[26] = {
        0, /* A */
        1, /* B */
        1, /* C */
        1, /* D */
        0, /* E */
        0, /* F */
        1, /* G */
        0, /* H */
        0, /* I */
        1, /* J */
        1, /* K */
        0, /* L */
        0, /* M */
        0, /* N */
        0, /* O */
        1, /* P */
        1, /* Q */
        0, /* R */
        0, /* S */
        1, /* T */
        0, /* U */
        1, /* V */
        1, /* W */
        0, /* X */
        1, /* Y */
        1  /* Z */
    };

    /* The sequence is only defined for the first 26 letters.
       For indices out of range we return 0 by convention. */
    if (n < 1 || n > 26)
        return (seq_t)0;
    return vals[n - 1];
}
