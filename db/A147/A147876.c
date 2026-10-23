/*
 * A147876  Smallest nonnegative integer containing the n-th letter of the
 *          alphabet (in French), or -1 if no such number exists.
 * https://oeis.org/A147876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, sign, word
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 1, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest nonnegative integer whose French spelling
 * contains the n‑th letter of the alphabet (1‑based).
 * Returns -1 if no such number exists (for J, K, W, Y).
 * Special case: the letter 'b' first appears at 10^12 ("un billion").
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* French spellings for 0..16 */
static const char * const units[17] = {
    "zero", "un", "deux", "trois", "quatre",
    "cinq", "six", "sept", "huit", "neuf",
    "dix", "onze", "douze", "treize", "quatorze",
    "quinze", "seize"
};

/* Tens words for 20,30,40,50,60 */
static const char * const tens_words[5] = {
    "vingt", "trente", "quarante", "cinquante", "soixante"
};

/* Helper: append a string to buf at position *pos */
static void append_str(char *buf, size_t *pos, const char *s)
{
    while (*s) buf[(*pos)++] = *s++;
}

/* Recursive routine: write French spelling of n (n > 0)
 * into buf starting at *pos.  buf must be large enough. */
static void write_number_uint64(uint64_t n, char *buf, size_t *pos)
{
    /* 0..16 */
    if (n < 17) {
        append_str(buf, pos, units[n]);
        return;
    }
    /* 17..19 */
    if (n < 20) {
        append_str(buf, pos, "dix-");
        append_str(buf, pos, units[n - 10]);
        return;
    }
    /* 20..69 */
    if (n < 70) {
        uint8_t tens = n / 10;          /* 2…6 */
        uint8_t rem  = n % 10;
        append_str(buf, pos, tens_words[tens - 2]);
        if (rem) {
            buf[(*pos)++] = '-';
            write_number_uint64(rem, buf, pos);
        }
        return;
    }
    /* 70..79 */
    if (n < 80) {
        append_str(buf, pos, "soixante-");
        write_number_uint64(n - 60, buf, pos);   /* 10..19 */
        return;
    }
    /* 80..99 */
    if (n < 100) {
        append_str(buf, pos, "quatre-vingt");
        if (n == 80) {
            buf[(*pos)++] = 's';           /* plural */
            return;
        }
        buf[(*pos)++] = '-';
        write_number_uint64(n - 80, buf, pos);   /* 1..19 */
        return;
    }
    /* 100..999 */
    if (n < 1000) {
        uint8_t hundreds = n / 100;
        uint16_t rem = n % 100;
        if (hundreds == 1) {
            append_str(buf, pos, "cent");
            if (rem) { buf[(*pos)++] = ' '; }
            else return;
        } else {
            write_number_uint64(hundreds, buf, pos);
            buf[(*pos)++] = ' ';
            append_str(buf, pos, "cent");
            if (rem) { buf[(*pos)++] = ' '; }
            else {
                buf[(*pos)++] = 's';      /* plural */
                return;
            }
        }
        if (rem) write_number_uint64(rem, buf, pos);
        return;
    }
    /* 1 000 … 999 999 */
    if (n < 1000000ULL) {
        uint32_t thousands = n / 1000;
        uint32_t rem = n % 1000;
        if (thousands == 1) {
            append_str(buf, pos, "mille");
            if (rem) { buf[(*pos)++] = ' '; }
            else return;
        } else {
            write_number_uint64(thousands, buf, pos);
            buf[(*pos)++] = ' ';
            append_str(buf, pos, "mille");
            if (rem) { buf[(*pos)++] = ' '; }
            else return;
        }
        if (rem) write_number_uint64(rem, buf, pos);
        return;
    }
    /* 1 000 000 … 999 999 999 */
    if (n < 1000000000ULL) {
        uint32_t millions = n / 1000000ULL;
        uint32_t rem = n % 1000000ULL;
        if (millions == 1) {
            append_str(buf, pos, "million");
            if (rem) { buf[(*pos)++] = ' '; }
            else return;
        } else {
            write_number_uint64(millions, buf, pos);
            buf[(*pos)++] = ' ';
            append_str(buf, pos, "millions");
            if (rem) { buf[(*pos)++] = ' '; }
            else return;
        }
        if (rem) write_number_uint64(rem, buf, pos);
        return;
    }
    /* 1 000 000 000 … 999 999 999 999 */
    if (n < 1000000000000ULL) {
        uint64_t billions = n / 1000000000ULL;
        uint32_t rem = n % 1000000000ULL;
        if (billions == 1) {
            append_str(buf, pos, "billion");
            if (rem) { buf[(*pos)++] = ' '; }
            else return;
        } else {
            write_number_uint64(billions, buf, pos);
            buf[(*pos)++] = ' ';
            append_str(buf, pos, "billions");
            if (rem) { buf[(*pos)++] = ' '; }
            else return;
        }
        if (rem) write_number_uint64(rem, buf, pos);
        return;
    }
    /* Numbers >= 10^12 are not needed for this sequence. */
    *pos = 0;
}

/* Test whether the French spelling of n contains the letter target. */
static bool contains_letter_uint64(uint64_t n, char target)
{
    char buf[256];
    size_t pos = 0;

    if (n == 0) {
        append_str(buf, &pos, "zero");
    } else {
        write_number_uint64(n, buf, &pos);
    }

    for (size_t i = 0; i < pos; ++i) {
        if (buf[i] == target) return true;
    }
    return false;
}

/* Memoisation for the 26 letters */
static seq_t memo[26];
static bool computed[26] = { false };

seq_t a(idx_t n)
{
    if (n < 1 || n > 26) return -1;          /* out of alphabet range */

    unsigned idx = (unsigned)(n - 1);          /* 0 … 25 */
    if (computed[idx]) return memo[idx];

    char target = 'a' + idx;
    seq_t result = -1;  /* default for “not found” */

    /* Special cases where the answer is known. */
    if (target == 'b') {                     /* 1 000 000 000 000 ("un billion") */
        result = (seq_t)1000000000000ULL;
    } else if (target == 'j' || target == 'k' ||
               target == 'w' || target == 'y') {
        result = -1;
    } else {
        /* Search from 0 upwards until the target letter appears. */
        for (uint64_t i = 0; ; ++i) {
            if (contains_letter_uint64(i, target)) {
                result = (seq_t)i;
                break;
            }
            /* Guard against an unlikely infinite loop if the letter does not exist. */
            if (i == 1000000000ULL) {   /* beyond practical search limit */
                result = -1;
                break;
            }
        }
    }

    memo[idx] = result;
    computed[idx] = true;
    return result;
}
