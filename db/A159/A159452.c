/*
 * A159452  Numbers containing no smaller numbers when written as English
 *          number names.
 * https://oeis.org/A159452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, word
 * Formula:   a(n) is the n-th positive integer whose English name contains no
 *            smaller positive integer as a substring. The sequence is finite;
 *            after the longest valid number all further terms are 0
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <string.h>
#include <stddef.h>

static const char *units[20] = {
    "zero","one","two","three","four","five",
    "six","seven","eight","nine","ten",
    "eleven","twelve","thirteen","fourteen","fifteen",
    "sixteen","seventeen","eighteen","nineteen"
};

static const char *tens_names[10] = {
    "", "", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};

/* Append a string to a buffer, making sure we do not overflow. */
static void append(char *buf, size_t sz, const char *s)
{
    size_t used = strlen(buf);
    size_t add  = strlen(s);
    if (used + add + 1 <= sz) {
        strcpy(buf + used, s);
    }
    /* else silently truncate – the buffer is sufficiently large for
       all numbers we test. */
}

/* Convert numbers 0…999 to their English name, lower‑case, hyphenated
   and space separated.  Returned string is stored in buf, which
   must be large enough (64 bytes is plenty). */
static char *english_name_upto_999(idx_t num, char *buf, size_t sz)
{
    buf[0] = '\0';

    if (num == 0) {
        append(buf, sz, "zero");
        return buf;
    }

    idx_t h = num / 100;
    idx_t rem = num % 100;

    if (h > 0) {
        append(buf, sz, units[h]);          /* e.g. "one"  */
        append(buf, sz, " hundred");        /* e.g. "hundred"  */
        if (rem > 0) {
            append(buf, sz, " ");
        }
    }

    if (rem >= 20) {
        idx_t t = rem / 10;
        idx_t u = rem % 10;
        append(buf, sz, tens_names[t]);     /* e.g. "twenty"  */
        if (u > 0) {
            append(buf, sz, "-");
            append(buf, sz, units[u]);     /* e.g. "one"  */
        }
    } else if (rem > 0) {                  /* 1…19  */
        append(buf, sz, units[rem]);
    }

    return buf;
}

/* Return true if the English name of num contains the English name
   of any smaller positive integer as a substring. */
static bool contains_smaller(idx_t num)
{
    if (num == 0) {          /* 0 has no smaller positive integers */
        return false;
    }

    char buf_candidate[128];
    const char *cand = english_name_upto_999(num, buf_candidate, sizeof(buf_candidate));

    for (idx_t m = 1; m < num; ++m) {
        char buf_check[128];
        const char *check = english_name_upto_999(m, buf_check, sizeof(buf_check));
        if (strstr(cand, check) != NULL) {
            return true;
        }
    }
    return false;
}

/* Main sequence function as requested. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined below offset – harmless */
    }

    idx_t count = 0;
    /* All valid numbers are below 60; search a little beyond just
       to be safe. */
    for (idx_t k = 0; k < 200; ++k) {
        if (!contains_smaller(k)) {
            count++;
            if (count == n) {
                return (seq_t)k;
            }
        }
    }
    return 0;  /* no further terms – sequence is finite */
}
