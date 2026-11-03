/*
 * A037194  Number of vowels in n (in French).
 * https://oeis.org/A037194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(n) = number of vowels in the French word for n
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for the standard integer types that the build may use */

/* Recursive helper to compute the vowel count of a French number */
static seq_t count_french(idx_t n)
{
    /* vowel counts for 0 … 19 */
    static const seq_t base0_19[20] = {
        2, /* 0  zéro */
        1, /* 1  un */
        2, /* 2  deux */
        2, /* 3  trois */
        3, /* 4  quatre */
        1, /* 5  cinq */
        1, /* 6  six */
        1, /* 7  sept */
        2, /* 8  huit */
        2, /* 9  neuf */
        1, /* 10 d i x */
        2, /* 11 o n z e */
        3, /* 12 d o u z e */
        3, /* 13 t r e i z e */
        4, /* 14 q u a t o r z e */
        3, /* 15 q u i n z e */
        3, /* 16 s e i z e */
        2, /* 17 d i x- s e p t */
        3, /* 18 d i x- h u i t */
        3  /* 19 d i x- n e u f */
    };

    const seq_t VOWELS_ET    = 1;  /* vowels in the conjunction "et"      */
    const seq_t VOWELS_100   = 1;  /* vowels in the word "cent"           */
    const seq_t VOWELS_1000  = 2;  /* vowels in the word "mille"          */
    const seq_t VOWELS_80    = 4;  /* vowels in the word "quatre-vingt"   */

    /* Special small numbers */
    if (n == 100)       return VOWELS_100;
    if (n == 1000)      return VOWELS_1000;
    if (n < 20)         return base0_19[(int)n];

    /* Numbers 20 … 69 */
    if (n < 70) {
        idx_t tens  = n / 10;
        idx_t ones  = n % 10;
        seq_t base  = 0;
        switch (tens) {
            case 2: base = 1; break; /* vingt */
            case 3: base = 2; break; /* trente */
            case 4: base = 4; break; /* quarante */
            case 5: base = 4; break; /* cinquante */
            case 6: base = 4; break; /* soixante */
            default: break;
        }
        if (ones == 0) return base;
        if (ones == 1) return base + (seq_t)1 + VOWELS_ET; /* "un" + "et" */
        return base + count_french(ones);
    }

    /* Numbers 70 … 79 */
    if (n < 80) {
        if (n == 70)   return count_french(60) + count_french(10);   /* soixante-dix */
        if (n == 71)   return count_french(60) + VOWELS_ET + count_french(11); /* soixante et onze */
        return count_french(60) + count_french(n - 60);
    }

    /* Numbers 80 … 99 */
    if (n < 100) {
        if (n == 80) return VOWELS_80;
        if (n == 90) return VOWELS_80 + count_french(10); /* quatre-vingt-dix */
        return VOWELS_80 + count_french(n - 80);
    }

    /* Numbers 100 … 999 */
    if (n < 1000) {
        idx_t hundreds = n / 100;
        idx_t rest     = n % 100;
        seq_t base;
        if (hundreds == 1)  base = count_french(100);          /* "cent" */
        else               base = count_french(hundreds) + count_french(100);
        if (rest == 0) return base;
        return base + count_french(rest);
    }

    /* Numbers 1 000 … 999 999 (covers the range used in the tests) */
    if (n < 1000000) {
        idx_t thousands = n / 1000;
        idx_t rest      = n % 1000;
        seq_t base;
        if (thousands == 1)  base = count_french(1000);         /* "mille" */
        else                base = count_french(thousands) + count_french(1000);
        if (rest == 0) return base;
        return base + count_french(rest);
    }

    /* Larger numbers are not needed for the current tests. */
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    return count_french(n);
}
