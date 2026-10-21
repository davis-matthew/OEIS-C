/*
 * A072959  Using the US English names for the nonnegative integers, assign
 *          each letter a numerical value as in A073327 (A=1, B=2, ..., Z=26),
 *          treat the name as a base-27 integer, and convert to decimal.
 * https://oeis.org/A072959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   append_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the base‑27 value of the U.S. English name of n with
 * letters mapped as A=1, B=2, …, Z=26 and space mapped to 0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/append_word.h"

/* Basic English word tables */
static const char *const units[10] = {
    "ZERO","ONE","TWO","THREE","FOUR","FIVE","SIX","SEVEN","EIGHT","NINE"
};
static const char *const teens[10] = {
    "TEN","ELEVEN","TWELVE","THIRTEEN","FOURTEEN","FIFTEEN",
    "SIXTEEN","SEVENTEEN","EIGHTEEN","NINETEEN"
};
static const char *const tens[10] = {
    "", "", "TWENTY","THIRTY","FORTY","FIFTY","SIXTY",
    "SEVENTY","EIGHTY","NINETY"
};

/* Scale names and values */
struct Scale {
    const char *name;
    unsigned long long value;
};

static const struct Scale scales[] = {
    {"TRILLION", 1000000000000ULL},
    {"BILLION",  1000000000ULL},
    {"MILLION",   1000000ULL},
    {"THOUSAND",   1000ULL}
};
static const unsigned long long num_scales = sizeof(scales)/sizeof(scales[0]);

/* Write the word for a number < 1000 into the buffer */
static void write_under_1000(unsigned long long n, char *buf, size_t *len)
{
    if (n == 0) return;

    if (n >= 100) {
        unsigned long long h = n / 100;
        append_word(units[h], buf, len);         /* digit word */
        append_word("HUNDRED", buf, len);        /* HUNDRED */
        n %= 100;
    }

    if (n >= 20) {
        unsigned long long t = n / 10;
        append_word(tens[t], buf, len);
        n %= 10;
    }

    if (n >= 10) {
        append_word(teens[n - 10], buf, len);
    } else if (n > 0) {
        append_word(units[n], buf, len);
    }
}

/* Convert a nonnegative integer n into its US English name */
static void number_to_words(unsigned long long n, char *buf)
{
    if (n == 0) {
        strcpy(buf, "ZERO");
        return;
    }

    size_t len = 0;

    for (unsigned int i = 0; i < num_scales; ++i) {
        unsigned long long v = scales[i].value;
        if (n >= v) {
            unsigned long long group = n / v;
            write_under_1000(group, buf, &len);          /* group word(s) */
            append_word(scales[i].name, buf, &len);       /* scale name */
            n -= group * v;
        }
    }

    write_under_1000(n, buf, &len);
    buf[len] = '\0';
}

/* Main sequence function */
seq_t a(idx_t n)
{
    char words[256];
    number_to_words((unsigned long long)n, words);

    seq_t val = 0;
    for (const char *p = words; *p; ++p) {
        unsigned d = (*p == ' ') ? 0 : (unsigned)(*p - 'A' + 1);
        val = val * 27 + (seq_t)d;
    }
    return val;
}
