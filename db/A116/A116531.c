/*
 * A116531  Number of letters in common in the US English names for the
 *          numbers n and n+1.
 * https://oeis.org/A116531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = number of letters (case‑insensitive) common to the US
 *            English word for n and for n+1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Helper routine: add the letter frequencies of a word string to an  */
/* array of 26 counters.  Only lower‑case letters are counted.         */
/* ------------------------------------------------------------------ */
static void add_word(const char *w, unsigned int *freq)
{
    while (*w) {
        char c = *w++;
        if ('a' <= c && c <= 'z')
            ++freq[(unsigned)(c - 'a')];
    }
}

/* ------------------------------------------------------------------ */
/* Build the letter frequency table for the US English name of an    */
/* unsigned 64‑bit integer n.  The name is constructed from decimal  */
/* groups of three digits (hundred‑thousands etc.).  All names are    */
/* generated in lower case.                                            */
/* ------------------------------------------------------------------ */
static void get_letter_frequencies(idx_t n, unsigned int *freq)
{
    /* Clear the frequency table. */
    memset(freq, 0, 26 * sizeof(*freq));

    /* Special case for zero. */
    if (n == 0) {
        add_word("zero", freq);
        return;
    }

    /* English word components. */
    static const char *UNITS[10]  = {"zero","one","two","three","four",
                                     "five","six","seven","eight","nine"};
    static const char *TEENS[10]  = {"ten","eleven","twelve","thirteen",
                                     "fourteen","fifteen","sixteen",
                                     "seventeen","eighteen","nineteen"};
    static const char *TENS[10]   = {"",  "",  "twenty","thirty","forty",
                                     "fifty","sixty","seventy","eighty","ninety"};
    static const char *THOUSANDS[4] = {"thousand","million",
                                       "billion","trillion"};

    /* Split the number into groups of three digits. */
    enum { MAX_GROUPS = 10 };          /* supports numbers up to 10^30 */
    idx_t groups[MAX_GROUPS];
    size_t group_count = 0;
    idx_t tmp = n;

    while (tmp > 0 && group_count < MAX_GROUPS) {
        groups[group_count++] = tmp % 1000;
        tmp /= 1000;
    }

    /* Process each group from highest to lowest. */
    for (size_t k = group_count; k > 0; --k) {
        size_t i = k - 1;          /* index of current group */
        idx_t chunk = groups[i];

        /* Skip groups that are zero. */
        if (chunk == 0)
            continue;

        /* Hundreds place. */
        if (chunk >= 100) {
            idx_t h = chunk / 100;
            add_word(UNITS[h], freq);     /* 1–9 */
            add_word("hundred", freq);
            chunk %= 100;
        }

        /* Tens and units. */
        if (chunk >= 20) {
            idx_t t = chunk / 10;
            add_word(TENS[t], freq);
            chunk %= 10;
        }
        if (chunk >= 10) {            /* 10–19 */
            add_word(TEENS[chunk - 10], freq);
            chunk = 0;
        }
        if (chunk > 0) {              /* 1–9 */
            add_word(UNITS[chunk], freq);
        }

        /* Thousand, million, ... suffix. */
        if (i > 0) {
            add_word(THOUSANDS[i - 1], freq);
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    unsigned int freq1[26];
    unsigned int freq2[26];

    get_letter_frequencies(n,     freq1);
    get_letter_frequencies(n + 1, freq2);

    /* Count letters common to both words. */
    unsigned int common = 0;
    for (size_t i = 0; i < 26; ++i) {
        common += (freq1[i] < freq2[i]) ? freq1[i] : freq2[i];
    }

    return (seq_t)common;
}
