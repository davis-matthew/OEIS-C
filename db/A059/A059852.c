/*
 * A059852  Consider the English alphabet in Morse code (the International
 *          Morse radio telegraph code). Map a 'dit' to the digit one and a
 *          'dah' to the digit 2, then express that ternary number in decimal.
 * https://oeis.org/A059852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn, word
 * Formula:   a(n) = value of Morse code for the n‑th letter of the alphabet,
 *            with dit = 1, dah = 2, interpreted as a base‑3 number
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n < 1 or n > 26 the function returns 0. */

seq_t a(idx_t n)
{
    /* Morse code strings for the 26 letters.  '.' → 1, '-' → 2. */
    static const char *morse[26] = {
        ".-",    /* A */
        "-...",  /* B */
        "-.-.",  /* C */
        "-..",   /* D */
        ".",     /* E */
        "..-.",  /* F */
        "--.",   /* G */
        "....",  /* H */
        "..",    /* I */
        ".---",  /* J */
        "-.-",   /* K */
        ".-..",  /* L */
        "--",    /* M */
        "-.",    /* N */
        "---",   /* O */
        ".--.",  /* P */
        "--.-",  /* Q */
        ".-.",   /* R */
        "...",   /* S */
        "-",     /* T */
        "..-",   /* U */
        "...-",  /* V */
        ".--",   /* W */
        "-..-",  /* X */
        "-.--",  /* Y */
        "--.."   /* Z */
    };

    if (n < 1 || n > 26)
        return (seq_t)0;                 /* undefined beyond the alphabet */

    const char *code = morse[n - 1];
    seq_t val = 0;
    for (const char *p = code; *p; ++p) {
        uint8_t digit = (*p == '.') ? 1 : 2;   /* dit = 1, dah = 2 */
        val = val * 3 + digit;
    }
    return val;
}
