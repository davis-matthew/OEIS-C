/*
 * A168310  The first thousand positive integers arranged alphabetically.
 * https://oeis.org/A168310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, look, fini, full
 * Formula:   a(n) is the nth term in the alphabetic ordering of 1..1000
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Number to English word (no “and”, spaces only). */
static const char *const units[20] = {
    "", "one", "two", "three", "four", "five",
    "six", "seven", "eight", "nine", "ten",
    "eleven", "twelve", "thirteen", "fourteen",
    "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};
static const char *const tens[10] = {
    "", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};
#define WORD_BUF 64

static void number_to_words(idx_t n, char *buf)
{
    if (n == 1000) {
        strcpy(buf, "one thousand");
        return;
    }

    int h = n / 100;
    int rem = n % 100;
    size_t pos = 0;

    if (h > 0) {
        pos += snprintf(buf + pos, WORD_BUF - pos, "%s hundred", units[h]);
    }

    if (rem > 0) {
        if (h > 0) {
            buf[pos++] = ' ';
        }

        if (rem < 20) {
            pos += snprintf(buf + pos, WORD_BUF - pos, "%s", units[rem]);
        } else {
            int t = rem / 10;
            int u = rem % 10;
            pos += snprintf(buf + pos, WORD_BUF - pos, "%s", tens[t]);
            if (u > 0) {
                buf[pos++] = ' ';
                pos += snprintf(buf + pos, WORD_BUF - pos, "%s", units[u]);
            }
        }
    }

    buf[pos] = '\0';
}

/* Global data */
static bool initialized = false;
static idx_t order[1000];
static char words[1000][WORD_BUF];

/* Comparator for qsort */
static int cmp_words(const void *a, const void *b)
{
    idx_t ia = *(const idx_t *)a;
    idx_t ib = *(const idx_t *)b;
    /* indices are 1-based, convert to 0-based */
    return strcmp(words[ia - 1], words[ib - 1]);
}

/* Build data once */
static void init(void)
{
    for (idx_t i = 1; i <= 1000; ++i) {
        number_to_words(i, words[i - 1]);
        order[i - 1] = i;
    }
    qsort(order, 1000, sizeof(idx_t), cmp_words);
    initialized = true;
}

/* Main function required by the challenge */
seq_t a(idx_t n)
{
    if (!initialized) {
        init();
    }
    if (n < 1 || n > 1000) {
        return (seq_t)0;
    }
    return (seq_t)order[n - 1];
}
