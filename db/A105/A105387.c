/*
 * A105387  Morse code alphabet where "-" = 0 and "." = 1, binary converted to
 *          decimal.
 * https://oeis.org/A105387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, fini
 * Formula:   a(n) = decimal value of the Morse code for the nth letter (A=1,
 *            B=2, ..., Z=26) where '.'=1, '-'=0
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is out of the alphabet range, return 0. */
    if (n < 1 || n > 26) return (seq_t)0;

    /* Morse representations for A..Z.  '.' = 1, '-' = 0. */
    static const char *morse[26] = {
            ".-",     /* A */
            "-...",   /* B */
            "-.-.",   /* C */
            "-..",    /* D */
            ".",      /* E */
            "..-.",   /* F */
            "--.",    /* G */
            "....",   /* H */
            "..",     /* I */
            ".---",   /* J */
            "-.-",    /* K */
            ".-..",   /* L */
            "--",     /* M */
            "-.",     /* N */
            "---",    /* O */
            ".--.",   /* P */
            "--.-",   /* Q */
            ".-.",    /* R */
            "...",    /* S */
            "-",      /* T */
            "..-",    /* U */
            "...-",   /* V */
            ".--",    /* W */
            "-..-",   /* X */
            "-.--",   /* Y */
            "--.."    /* Z */
    };

    const char *s = morse[n - 1];
    seq_t val = 0;
    for (const char *p = s; *p; ++p) {
        /* Multiply by 2 (left shift) and add the bit. */
        val = val * (seq_t)2 + (*p == '.' ? (seq_t)1 : (seq_t)0);
    }
    return val;
}
