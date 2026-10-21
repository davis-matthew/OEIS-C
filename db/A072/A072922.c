/*
 * A072922  Spell English name for n, then interpret as number in base 36.
 * https://oeis.org/A072922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  word, base, nonn
 * Formula:   a(n) = value of the English name of n interpreted as a base‑36
 *            number * (digits a‑z are mapped to 10‑35). The function builds
 *            the English * cardinal representation of n, strips all
 *            non‑letters, and evaluates the * remaining string as a base‑36
 *            integer. All arithmetic uses the supplied * seq_t type
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <ctype.h>    /* for isalpha */

/* English words for 0‑19  */
static const char *const ones[] =
{
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

/* English words for the tens 20‑90 */
static const char *const tens[] =
{
    "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};

/* Scale names for 10^3, 10^6, … , 10^18 */
static const char *const scales[] =
{
    "",           /* units 10^0 */
    "thousand",   /* 10^3 */
    "million",    /* 10^6 */
    "billion",    /* 10^9 */
    "trillion",   /* 10^12 */
    "quadrillion",/* 10^15 */
    "quintillion" /* 10^18 */
};

/* --------------------------------------------------------------------- */
/* Helper: write a number < 1000 into a buffer (no leading spaces)       */
/* `buf` must have room for at least 64 bytes (enough for any 3‑digit part)*/
/* Returns the number of characters written.                              */
static size_t write_hundreds(uint64_t n, char *buf)
{
    size_t pos = 0;

    /* hundreds part */
    if (n >= 100) {
        unsigned h = n / 100;
        pos += sprintf(buf + pos, "%s", ones[h]);          /* “X” */
        pos += sprintf(buf + pos, " hundred");             /* “ hundred” */
        n %= 100;
        if (n > 0)
            pos += sprintf(buf + pos, " ");                /* spacer */
    }

    /* tens and units */
    if (n > 0) {
        if (n < 20) {                     /* 1‑19 */
            pos += sprintf(buf + pos, "%s", ones[n]);
        } else {                           /* 20‑99 */
            unsigned t = n / 10;
            unsigned u = n % 10;
            pos += sprintf(buf + pos, "%s", tens[t - 2]);  /* 20→twenty */
            if (u > 0)
                pos += sprintf(buf + pos, " %s", ones[u]); /* add unit */
        }
    }

    return pos;
}

/* --------------------------------------------------------------------- */
/* Convert a non‑negative integer to its English cardinal words.         */
/* `buf` must be large enough (512 bytes is more than enough).           */
static size_t convert_to_words(uint64_t n, char *buf)
{
    if (n == 0) {
        strcpy(buf, "zero");
        return 4;
    }

    /* Temporary storage for each 3‑digit group */
    char part[64];
    size_t part_len;
    size_t i = 0;
    size_t out_pos = 0;
    uint64_t tmp = n;
    unsigned scale_idx = 0;

    /* Gather parts from least significant to most (later reversed) */
    while (tmp > 0) {
        uint64_t group = tmp % 1000;
        if (group != 0) {
            part_len = write_hundreds(group, part);        /* build part */
            if (scale_idx > 0) {                          /* add scale */
                part_len += sprintf(part + part_len, " %s",
                                    scales[scale_idx]);
            }
            /* prepend this part to the final buffer (grow from end) */
            out_pos += part_len;
            memmove(buf + (part_len), buf, out_pos - part_len);
            memcpy(buf, part, part_len);
        }
        tmp /= 1000;
        ++scale_idx;
    }

    return out_pos;
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                                 */
seq_t a(idx_t n)
{
    /* English words for n */
    char word[512];
    size_t word_len = convert_to_words(n, word);

    /* Convert letters to base‑36 value */
    seq_t result = 0;
    for (size_t i = 0; i < word_len; ++i) {
        char c = word[i];
        if (isalpha((unsigned char)c)) {
            int digit = (c >= 'a') ? (c - 'a' + 10) : (c - 'A' + 10);
            result = result * 36 + digit;
        }
    }
    return result;
}
