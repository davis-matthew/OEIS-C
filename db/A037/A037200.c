/*
 * A037200  Number of vowels in n (in German).
 * https://oeis.org/A037200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   number of vowels in the German word for integer n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Vowel counts for 0‑19 */
static const seq_t UNITS[20] = {
    1, /* null        */
    2, /* ein         */
    2, /* zwei        */
    2, /* drei        */
    2, /* vier        */
    1, /* fünf        */
    1, /* sechs       */
    3, /* sieben      */
    1, /* acht        */
    2, /* neun        */
    1, /* zehn        */
    1, /* elf         */
    1, /* zwölf       */
    3, /* dreizehn    */
    3, /* vierzehn    */
    2, /* fünfzehn    */
    2, /* sechzehn    */
    3, /* siebzehn    */
    2, /* achtzehn    */
    3  /* neunzehn    */
};

/* Vowel counts for the unit part when used with a tens word */
static const seq_t UNIT_COMPOUND[10] = {
    0, /* unused */
    2, /* ein */
    2, /* zwei */
    2, /* drei */
    2, /* vier */
    1, /* fünf */
    1, /* sechs */
    3, /* sieben */
    1, /* acht */
    2  /* neun */
};

/* Vowel counts for the tens words (0 unused) */
static const seq_t TENS[10] = {
    0, /* unused */
    0, /* 10, handled separately */
    2, /* zwanzig  */
    3, /* dreißig  */
    3, /* vierzig  */
    2, /* fünfzig  */
    2, /* sechzig  */
    3, /* siebzig  */   /* corrected from 4 to 3 */
    2, /* achtzig  */
    3  /* neunzig  */
};

static const seq_t UND        = 1;   /* "und"          */
static const seq_t HUNDERT    = 2;   /* "hundert"      */
static const seq_t EINHUNDERT = 4;   /* "einhundert"   */
static const seq_t TAUSEND    = 3;   /* "tausend"      */
static const seq_t EIN        = 2;   /* "ein"          */
static const seq_t EINE       = 3;   /* "eine"         */

/* Singular and plural vowel counts for thousands, millions, etc. */
static const seq_t SING[5] = { 3, 4, 3, 4, 3 }; /* Million, Milliarden, ... */
static const seq_t PLUR[5] = { 4, 4, 4, 4, 4 }; /* Millionen, ...      */

/* Count vowels for numbers < 1000 */
static seq_t count_word(idx_t n)
{
    if (n < 20)                   /* units */
        return UNITS[n];

    if (n < 100) {                /* tens */
        idx_t tens  = n / 10;
        idx_t units = n % 10;
        if (units == 0)
            return TENS[tens];
        return UNIT_COMPOUND[units] + UND + TENS[tens];
    }

    /* 100 – 999 */
    idx_t hundreds = n / 100;
    idx_t rest     = n % 100;
    seq_t result = (hundreds == 1) ? EINHUNDERT
                                   : (UNIT_COMPOUND[hundreds] + HUNDERT);
    if (rest)
        result += count_word(rest);
    return result;
}

/* Count vowels for any integer n >= 0 */
static seq_t count_generic(idx_t n)
{
    if (n == 0)
        return UNITS[0];          /* "null" */

    seq_t total = 0;
    idx_t remaining = n;
    int exp = 0;                 /* 0 = units, 1 = thousands, 2 = millions, ... */

    const idx_t original = n;    /* needed for the special 100 rule */

    while (remaining) {
        idx_t group = remaining % 1000;   /* current 3‑digit block */

        if (group) {
            seq_t part;
            seq_t base;

            if (exp == 0) {                 /* units block */
                /* 100 is spelled "hundert" when part of a larger number */
                if (group == 100 && original >= 1000)
                    base = HUNDERT;
                else
                    base = count_word(group);
                part = base;
            } else {                        /* thousands or higher */
                base = count_word(group);

                if (exp == 1) {             /* thousands */
                    if (group == 1)
                        part = TAUSEND;
                    else
                        part = base + TAUSEND;
                } else {                    /* millions and above */
                    if (group == 1)
                        part = EINE + SING[exp - 2];
                    else
                        part = base + PLUR[exp - 2];
                }
            }
            total += part;
        }

        remaining /= 1000;
        ++exp;
    }
    return total;
}

seq_t a(idx_t n)
{
    return count_generic(n);
}
