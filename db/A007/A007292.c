/*
 * A007292  Number of letters in n (in Hungarian).
 * https://oeis.org/A007292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  word, nonn
 * Formula:   a(n) = number of letters in the Hungarian name of n (gy, ny, sz,
 *            cs, zs, ly, ty count as one letter); the name is built from
 *            ones/tens/hundreds/thousands words, then counted
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   count_letters
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/count_letters.h"

/* Accented vowels are written as plain ASCII; each still counts as one letter. */
static const char *ones[] = {
    "", "egy", "ketto", "harom", "negy", "ot", "hat", "het", "nyolc", "kilenc"
};

static const char *tens[] = {
    "", "tiz", "husz", "harminc", "negyven", "otven", "hatvan", "hetven", "nyolcvan", "kilencven"
};

static const char *tens_prefix[] = {
    "", "tizen", "huszon", "harminc", "negyven", "otven", "hatvan", "hetven", "nyolcvan", "kilencven"
};

static void add_1_99(char *buffer, int num) {
    int t = num / 10;
    int o = num % 10;
    if (num == 0) return;
    if (o == 0) {
        strcat(buffer, tens[t]);
    } else {
        strcat(buffer, tens_prefix[t]);
        strcat(buffer, ones[o]);
    }
}

/* multiplier form: "ket" instead of "ketto" before szaz/ezer/millio */
static void add_mult_digit(char *buffer, int d) {
    if (d == 2) strcat(buffer, "ket");
    else strcat(buffer, ones[d]);
}

static void add_1_999(char *buffer, int num, int as_multiplier) {
    int h = num / 100;
    int r = num % 100;
    if (h > 0) {
        if (h > 1) add_mult_digit(buffer, h);
        strcat(buffer, "szaz");
    }
    if (r > 0) {
        if (as_multiplier && r % 10 == 2) {
            int t = r / 10;
            strcat(buffer, tens_prefix[t]);
            strcat(buffer, "ket");
        } else {
            add_1_99(buffer, r);
        }
    }
}

seq_t a(idx_t n) {
    char buffer[512] = "";
    static const char *scale[] = { "", "ezer", "millio", "milliard", "billio", "billiard", "trillio" };
    int groups[7];
    int ng = 0, g;
    idx_t m = n;

    if (n == 0) return (seq_t)count_letters("nulla");

    while (m > 0) {
        groups[ng++] = (int)(m % 1000);
        m /= 1000;
    }
    for (g = ng - 1; g >= 0; g--) {
        if (groups[g] == 0) continue;
        if (g == 0) {
            add_1_999(buffer, groups[g], 0);
        } else {
            if (!(g == 1 && groups[g] == 1))
                add_1_999(buffer, groups[g], 1);
            strcat(buffer, scale[g]);
        }
    }
    return (seq_t)count_letters(buffer);
}
