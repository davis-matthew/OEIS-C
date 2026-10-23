/*
 * A174880  Decimal representation of the consonants and vowels in the
 *          American English name of n mapped to binary 1 and 0.
 * https://oeis.org/A174880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 112, u32 n <= 1322, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A174880:  Decimal representation of consonants and vowels in the
 * English name of n, mapped to binary 1 (consonant) and 0 (vowel).
 * The binary string is interpreted as an unsigned integer. */
#include <stddef.h>

static const char *const ones[20] = {
    "zero",   "one",    "two",     "three",  "four",
    "five",   "six",    "seven",   "eight",  "nine",
    "ten",    "eleven", "twelve",  "thirteen","fourteen",
    "fifteen","sixteen","seventeen","eighteen","nineteen"
};

static const char *const tens[10] = {
    "",       "",       "twenty", "thirty", "forty",
    "fifty",  "sixty",  "seventy","eighty", "ninety"
};

/* Append a string literal to buf at position *pos */
static void append_str(char *buf, size_t *pos, const char *s)
{
    while (*s) buf[(*pos)++] = *s++;
}

/* Recursive routine that writes the English name of n into buf.
 * The caller must ensure enough buffer space. */
static void write_name(idx_t n, char *buf, size_t *pos)
{
    if (n == 0) return;               /* nothing for sub‑parts */

    if (n >= 1000) {
        idx_t thousands = n / 1000;
        write_name(thousands, buf, pos);
        append_str(buf, pos, " thousand");
        n %= 1000;
        if (n) {
            append_str(buf, pos, " ");
            write_name(n, buf, pos);
        }
        return;
    }

    if (n >= 100) {
        idx_t h = n / 100;
        write_name(h, buf, pos);
        append_str(buf, pos, " hundred");
        n %= 100;
        if (n) {
            append_str(buf, pos, " ");
            write_name(n, buf, pos);
        }
        return;
    }

    if (n >= 20) {
        idx_t t = n / 10;
        append_str(buf, pos, tens[t]);
        n %= 10;
        if (n) {
            append_str(buf, pos, " ");
            write_name(n, buf, pos);
        }
        return;
    }

    /* 0 – 19 */
    append_str(buf, pos, ones[n]);
}

/* External function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)10; /* 'zero' -> 1010b = 10 */
    }

    char buf[256];
    size_t pos = 0;

    /* Build the English name of n */
    write_name(n, buf, &pos);

    seq_t result = 0;

    /* Convert the gathered name into the binary value. */
    for (size_t i = 0; i < pos; ++i) {
        char c = buf[i];
        if (c == ' ' || c == '-') continue; /* ignore spaces/hyphens */

        /* Convert to lowercase if necessary */
        char lc = (c >= 'A' && c <= 'Z') ? (c + 32) : c;

        int bit = (lc == 'a' || lc == 'e' || lc == 'i' || lc == 'o' || lc == 'u') ? 0 : 1;
        result = (result << 1) | bit;
    }

    return result;
}
