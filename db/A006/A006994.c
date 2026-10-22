/*
 * A006994  Number of letters in n (in Russian).
 * https://oeis.org/A006994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters in the Russian cardinal name of n,
 *            built from the words (with the feminine одна/две and
 *            тысяча/тысячи/тысяч forms for thousands) and counted letter by
 *            letter in UTF-8, spaces excluded
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   ru_letters
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/ru_letters.h"

static const char *const ru_ones[10] = {
    "", "один", "два", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"
};
static const char *const ru_ones_fem[10] = {
    "", "одна", "две", "три", "четыре", "пять", "шесть", "семь", "восемь", "девять"
};
static const char *const ru_teens[10] = {
    "десять", "одиннадцать", "двенадцать", "тринадцать", "четырнадцать",
    "пятнадцать", "шестнадцать", "семнадцать", "восемнадцать", "девятнадцать"
};
static const char *const ru_tens[10] = {
    "", "", "двадцать", "тридцать", "сорок", "пятьдесят",
    "шестьдесят", "семьдесят", "восемьдесят", "девяносто"
};
static const char *const ru_hundreds[10] = {
    "", "сто", "двести", "триста", "четыреста", "пятьсот",
    "шестьсот", "семьсот", "восемьсот", "девятьсот"
};
/* scale words: [scale][form], form 0 = one, 1 = few (2-4), 2 = many */
static const char *const ru_scale[7][3] = {
    {"", "", ""},
    {"тысяча", "тысячи", "тысяч"},
    {"миллион", "миллиона", "миллионов"},
    {"миллиард", "миллиарда", "миллиардов"},
    {"триллион", "триллиона", "триллионов"},
    {"квадриллион", "квадриллиона", "квадриллионов"},
    {"квинтиллион", "квинтиллиона", "квинтиллионов"}
};

static int ru_group(unsigned g, int feminine) {
    int c = 0;
    c += ru_letters(ru_hundreds[g / 100]);
    g %= 100;
    if (g >= 10 && g < 20) {
        c += ru_letters(ru_teens[g - 10]);
    } else {
        c += ru_letters(ru_tens[g / 10]);
        c += ru_letters(feminine ? ru_ones_fem[g % 10] : ru_ones[g % 10]);
    }
    return c;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    int scale = 0;
    uint64_t m = (uint64_t)n;
    while (m > 0 && scale < 7) {
        unsigned g = (unsigned)(m % 1000);
        m /= 1000;
        if (g > 0) {
            if (scale == 0) {
                result += (seq_t)ru_group(g, 0);
            } else {
                unsigned last2 = g % 100, last = g % 10;
                int form;
                if (last2 >= 11 && last2 <= 14) form = 2;
                else if (last == 1) form = 0;
                else if (last >= 2 && last <= 4) form = 1;
                else form = 2;
                /* a bare 1000 is just "тысяча" (likewise "миллион", ...) */
                if (!(g == 1))
                    result += (seq_t)ru_group(g, scale == 1);
                result += (seq_t)ru_letters(ru_scale[scale][form]);
            }
        }
        scale++;
    }
    return result;
}
