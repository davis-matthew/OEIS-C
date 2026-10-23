/*
 * A105386  Morse code alphabet where "." = 0 and "-" = 1, binary converted to
 *          decimal.
 * https://oeis.org/A105386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, fini
 * Formula:   a(n) = decimal value of the Morse code pattern of the nth
 *            English alphabet character (A‑Z), where '.'→0 and '-'→1
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Conversion: read the code from left to right, left‑shifting the accumulated value and OR‑ing the bit defined by each symbol.  */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Morse code patterns for A to Z */
    static const char *const morse[26] = {
        ".-",   /* A */
        "-...", /* B */
        "-.-.", /* C */
        "-..",  /* D */
        ".",    /* E */
        "..-.", /* F */
        "--.",  /* G */
        "....", /* H */
        "..",   /* I */
        ".---", /* J */
        "-.-",  /* K */
        ".-..", /* L */
        "--",   /* M */
        "-.",   /* N */
        "---",  /* O */
        ".--.", /* P */
        "--.-", /* Q */
        ".-.",  /* R */
        "...",  /* S */
        "-",    /* T */
        "..-",  /* U */
        "...-", /* V */
        ".--",  /* W */
        "-..-", /* X */
        "-.--", /* Y */
        "--.."  /* Z */
    };

    /* Sequence defined only for 1 ≤ n ≤ 26.  Undefined indices return 0. */
    if (n == 0 || n > 26) {
        return 0;
    }

    const char *code = morse[n - 1];
    seq_t val = 0;

    for (const char *p = code; *p; ++p) {
        val = (val << 1) | (seq_t)(*p == '-');
    }

    return val;
}
