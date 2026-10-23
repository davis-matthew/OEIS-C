/*
 * A139212  Number of vowels in the preceding terms spelled out in French.
 * https://oeis.org/A139212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   vowelCountString
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139212: Number of vowels in the preceding terms spelled out in French.
   Recurrence: a(1) = 0
               a(n) = a(n-1) + v(a(n-1))   for n > 1
   where v(x) is the number of French vowels (a,e,i,o,u) in the spelled‑out word of x.
   Hyphens, accents and spaces are ignored while counting vowels. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/vowelCountString.h"

/* Constants for French number parts.
   unit_words[0] = "zero" for n==0.
   unit_words[1..9] are the regular unit words.                     */
static const char *const unit_words[10] =
{
    "zero", "un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf"
};

/* words for 10..16 */
static const char *const tens10_16[7] =
{
    "dix", "onze", "douze", "treize", "quatorze", "quinze", "seize"
};

/* words for the tens 20..60 */
static const char *const tens_words[5] =
{
    "vingt", "trente", "quarante", "cinquante", "soixante"
};

/* helper to count vowels in a number (idx_t) */
static seq_t countVowelsNumber(idx_t n)
{
    /* 0 -> "zero" */
    if (n == 0) return vowelCountString(unit_words[0]);

    /* 1..9 */
    if (n < 10) return vowelCountString(unit_words[n]);

    /* 10..16 */
    if (n < 17) return vowelCountString(tens10_16[n - 10]);

    /* 17..19 : "dix-" + unit */
    if (n < 20) return vowelCountString("dix") + vowelCountString(unit_words[n - 10]);

    /* 20..69 */
    if (n < 70) {
        idx_t tens   = n / 10;          /* 2..6 */
        idx_t units  = n % 10;
        seq_t sum = vowelCountString(tens_words[tens - 2]); /* tens word */
        if (units == 0) return sum;
        if (units == 1 && tens < 8) {   /* 21,31,41,51,61 : "vingt et un" etc. */
            return sum + vowelCountString("et") + vowelCountString(unit_words[1]);
        }
        return sum + vowelCountString(unit_words[units]);
    }

    /* 70..79 : "soixante" + (10..19) */
    if (n < 80) {
        seq_t base = vowelCountString("soixante");
        idx_t rem = n - 60;                 /* 10..19 */
        if (rem == 10) return base + vowelCountString("dix");            /* 70 */
        if (rem == 11) return base + vowelCountString("et") + vowelCountString("onze"); /* 71 */
        return base + countVowelsNumber(rem); /* 72..79 */
    }

    /* 80 */
    if (n == 80) return vowelCountString("quatre-vingt");

    /* 81..99 : "quatre-vingt-" + part */
    if (n < 100) {
        seq_t base = vowelCountString("quatre-vingt");
        idx_t rem = n - 80;                 /* 1..19 */
        return base + countVowelsNumber(rem);
    }

    /* 100..999 */
    if (n < 1000) {
        idx_t hundreds = n / 100;
        idx_t rem = n % 100;
        seq_t cent_vowels = vowelCountString("cent");   /* 1 */
        if (hundreds == 1) {
            if (rem == 0) return cent_vowels;          /* 100 */
            return cent_vowels + countVowelsNumber(rem);
        }
        seq_t base = vowelCountString(unit_words[hundreds]) + cent_vowels;
        if (rem == 0) return base;                    /* e.g., 200,300,... */
        return base + countVowelsNumber(rem);
    }

    /* 1000..9999 */
    if (n < 10000) {
        idx_t thousands = n / 1000;   /* 1..9 */
        idx_t rem = n % 1000;
        seq_t mille_vowels = vowelCountString("mille"); /* 2 */
        if (thousands == 1) {
            if (rem == 0) return mille_vowels;        /* 1000 */
            return mille_vowels + countVowelsNumber(rem);
        }
        seq_t base = vowelCountString(unit_words[thousands]) + mille_vowels;
        if (rem == 0) return base;                    /* e.g., 2000,3000,... */
        return base + countVowelsNumber(rem);
    }

    /* For numbers >= 10000 we extend using the same logic (handles up to millions) */
    /* Thousands of thousands: 10000 -> "dix mille" etc. */
    if (n < 1000000) {
        seq_t sum = 0;
        idx_t thousands = n / 1000;      /* up to 999 */
        if (thousands == 0) return countVowelsNumber(n % 1000);

        if (thousands == 1) {
            /* "dix mille", etc. */
            seq_t base = vowelCountString("dix") + vowelCountString("mille");
            if (n % 1000 == 0) return base;
            return base + countVowelsNumber(n % 1000);
        }

        /* arbitrary thousands (2..999) */
        seq_t base = countVowelsNumber(thousands) + vowelCountString("mille");
        if (n % 1000 == 0) return base;
        return base + countVowelsNumber(n % 1000);
    }

    /* Fallback: compute recursively using general rules. */
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;  /* a(1)=0 */

    seq_t val = 0;
    for (idx_t i = 1; i < n; ++i) {
        val += countVowelsNumber((idx_t)val);
    }
    return val;
}
