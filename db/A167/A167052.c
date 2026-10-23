/*
 * A167052  Scrabble value of the Spanish word for the number n.
 * https://oeis.org/A167052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, word
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(k) = scrabble value of the Spanish word representing k (0–…) */
#include <stdint.h>

static const seq_t letter_val[26] = {
    1,  /* A */
    3,  /* B */
    3,  /* C */
    2,  /* D */
    1,  /* E */
    4,  /* F */
    2,  /* G */
    4,  /* H */
    0,  /* I (value 0) */
    8,  /* J */
    5,  /* K */
    1,  /* L */
    3,  /* M */
    1,  /* N */
    1,  /* O */
    3,  /* P */
    5,  /* Q */
    1,  /* R */
    1,  /* S */
    1,  /* T */
    1,  /* U */
    4,  /* V */
    0,  /* W (unused) */
    8,  /* X */
    4,  /* Y */
    10  /* Z */
};

static void add_word_value(const char *w, seq_t *sum)
{
    for (const char *p = w; *p; ++p) {
        unsigned char c = *p;
        if (c >= 'a' && c <= 'z')
            c -= 32;          /* to upper case */
        else if (c < 'A' || c > 'Z')
            continue;         /* ignore non‑letters */
        *sum += (seq_t)letter_val[c - 'A'];
    }
}

static void add_number_part(idx_t n, seq_t *sum)
{
    if (n == 0) return;   /* nothing to add */

    /* hundreds */
    idx_t hundreds = n / 100;
    idx_t rem = n % 100;

    if (hundreds > 0) {
        if (hundreds == 1) {
            if (rem == 0)
                add_word_value("cien", sum);
            else
                add_word_value("ciento", sum);
        } else {
            const char *hundreds_words[10] = {
                "", "cien", "doscientos", "trescientos", "cuatrocientos",
                "quinientos", "seiscientos", "setecientos",
                "ochocientos", "novecientos"
            };
            add_word_value(hundreds_words[hundreds], sum);
        }
    }

    /* remainder 0–99 */
    if (rem == 0) return;

    static const char *unit_words[10] = {
        "", "uno", "dos", "tres", "cuatro", "cinco",
        "seis", "siete", "ocho", "nueve"
    };
    static const char *special_words[21] = {
        "", "uno", "dos", "tres", "cuatro", "cinco", "seis",
        "siete", "ocho", "nueve",
        "diez", "once", "doce", "trece", "catorce", "quince",
        "dieciseis", "diecisiete", "dieciocho", "diecinueve", "veinte"
    };
    static const char *tens_words[10] = {
        "", "", "veinte", "treinta", "cuarenta",
        "cincuenta", "sesenta", "setenta", "ochenta", "noventa"
    };

    if (rem < 20) {
        add_word_value(special_words[rem], sum);
    } else if (rem < 30) {
        if (rem == 20) {
            add_word_value("veinte", sum);
        } else {
            add_word_value("veint", sum);
            add_word_value(unit_words[rem % 10], sum);
        }
    } else {
        idx_t t = rem / 10;
        idx_t u = rem % 10;
        add_word_value(tens_words[t], sum);
        if (u) {
            add_word_value("y", sum);
            add_word_value(unit_words[u], sum);
        }
    }
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) {
        add_word_value("cero", &sum);
    } else {
        idx_t thousands = n / 1000;
        idx_t remainder = n % 1000;
        if (thousands) {
            add_number_part(thousands, &sum);
            add_word_value("mil", &sum);
        }
        if (remainder)
            add_number_part(remainder, &sum);
        /* For numbers between 1000 and 99999, thousands part may be > 9.
           The algorithm above correctly handles thousands up to 99 (i.e., 99999). 
           It recursively expands the thousands part via add_number_part. */
    }
    return sum;
}
