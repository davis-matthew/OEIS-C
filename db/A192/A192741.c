/*
 * A192741  Number of straight line segments in all letters of the capitalized
 *          Spanish name of n. Spanish version of A163828.
 * https://oeis.org/A192741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of straight line segments in the capitalized
 *            Spanish name of n
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int STROKE_MAP[26] = {
    3, /* A */
    1, /* B */
    0, /* C */
    1, /* D */
    4, /* E */
    3, /* F */
    2, /* G */
    3, /* H */
    1, /* I */
    1, /* J */
    3, /* K */
    2, /* L */
    4, /* M */
    3, /* N */
    0, /* O */
    1, /* P */
    1, /* Q */
    2, /* R */
    0, /* S */
    2, /* T */
    0, /* U */
    2, /* V */
    4, /* W */
    2, /* X */
    3, /* Y */
    3  /* Z */
};

static seq_t strokes_of_string(const char *s)
{
    seq_t sum = 0;
    for (const char *p = s; *p; ++p) {
        char c = *p;
        if (c >= 'A' && c <= 'Z')
            sum += STROKE_MAP[c - 'A'];
    }
    return sum;
}

static const char *UNITS[10] = { "CERO", "UNO", "DOS", "TRES", "CUATRO", "CINCO",
                                 "SEIS", "SIETE", "OCHO", "NUEVE" };

static const char *TEENS15[6] = { "DIEZ", "ONCE", "DOCE", "TRECE", "CATORCE",
                                  "QUINCE" };

static const char *TENS_WORD[10] = { "", "", "", "TREINTA", "CUARENTA",
                                    "CINCUENTA", "SESENTA", "SETENTA",
                                    "OCHENTA", "NOVENTA" };

static const char *HUNDREDS_WORD[10] = { "", "", "DOSCIENTOS", "TRESCIENTOS",
                                         "CUATROCIENTOS", "QUINIENTOS",
                                         "SEISCIENTOS", "SETECIENTOS",
                                         "OCHOCIENTOS", "NOVECIENTOS" };

static seq_t count0_999(idx_t n)
{
    if (n < 10) {          /* 1..9 */
        return strokes_of_string(UNITS[n]);
    }
    if (n < 20) {
        if (n <= 15)
            return strokes_of_string(TEENS15[n - 10]);    /* 10..15 */
        /* 16..19 : "DIECI" + unit */
        seq_t dieci = strokes_of_string("DIECI");
        return dieci + strokes_of_string(UNITS[n - 10]);
    }
    if (n < 30) {
        if (n == 20)
            return strokes_of_string("VEINTE");
        /* 21..29 : "VEINTI" + unit */
        seq_t veinti = strokes_of_string("VEINTI");
        return veinti + strokes_of_string(UNITS[n - 20]);
    }
    if (n < 100) {
        idx_t tens = n / 10;
        idx_t unit = n % 10;
        if (unit == 0)
            return strokes_of_string(TENS_WORD[tens]);
        return strokes_of_string(TENS_WORD[tens]) + strokes_of_string("Y")
               + strokes_of_string(UNITS[unit]);
    }
    /* 100..999 */
    idx_t hundreds = n / 100;
    idx_t remainder = n % 100;
    if (hundreds == 1) {
        if (remainder == 0)
            return strokes_of_string("CIEN");
        return strokes_of_string("CIENTO") + count0_999(remainder);
    }
    seq_t base = strokes_of_string(HUNDREDS_WORD[hundreds]);
    if (remainder == 0)
        return base;
    return base + count0_999(remainder);
}

static seq_t count_strokes(idx_t n)
{
    if (n == 0)
        return strokes_of_string("CERO");
    if (n < 1000)
        return count0_999(n);
    if (n < 1000000) {          /* thousands */
        idx_t thousand = n / 1000;
        idx_t rem = n % 1000;
        seq_t s;
        if (thousand == 1)
            s = strokes_of_string("MIL");
        else
            s = count_strokes(thousand) + strokes_of_string("MIL");
        if (rem > 0)
            s += count0_999(rem);
        return s;
    }
    if (n < 1000000000) {       /* millions */
        idx_t million = n / 1000000;
        idx_t rem = n % 1000000;
        seq_t s;
        if (million == 1)
            s = strokes_of_string("UN") + strokes_of_string("MILLON");
        else
            s = count_strokes(million) + strokes_of_string("MILLON");
        if (rem > 0)
            s += count_strokes(rem);
        return s;
    }
    /* very large numbers treat as billions (generic case) */
    idx_t giga = n / 1000000000;
    idx_t rem = n % 1000000000;
    seq_t s;
    if (giga == 1)
        s = strokes_of_string("UN") + strokes_of_string("BILLON");
    else
        s = count_strokes(giga) + strokes_of_string("BILLON");
    if (rem > 0)
        s += count_strokes(rem);
    return s;
}

seq_t a(idx_t n)
{
    return count_strokes(n);
}
