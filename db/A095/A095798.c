/*
 * A095798  Numbers whose name in English contains a "v".
 * https://oeis.org/A095798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) is the nth positive integer whose English name contains the
 *            * letter 'v'. The function generates numbers in order and tests
 *            each * number's English word for the presence of 'v', caching
 *            the results so * that arbitrary order of calls is handled
 *            efficiently
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>

#include "../utils/grow_seq_array.h"

/* English words for numbers 0‑19 */
static const char *under20[20] = {
    "zero","one","two","three","four","five","six","seven","eight","nine",
    "ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen",
    "seventeen","eighteen","nineteen"
};

/* English words for tens */
static const char *tens[10] = {
    "", "", "twenty", "thirty", "forty", "fifty",
    "sixty", "seventy", "eighty", "ninety"
};

/* Recursive routine that builds the English name for n (non‑zero). */
static void words_rec(uint64_t n, char *buf)
{
    if (n >= 1000000000ULL) {         /* billions */
        uint64_t billions = n / 1000000000ULL;
        words_rec(billions, buf);
        strncat(buf, " billion", 255);
        uint64_t rem = n % 1000000000ULL;
        if (rem) {
            if (strlen(buf) > 0) strncat(buf, " ", 255);
            words_rec(rem, buf);
        }
    } else if (n >= 1000000ULL) {      /* millions */
        uint64_t millions = n / 1000000ULL;
        words_rec(millions, buf);
        strncat(buf, " million", 255);
        uint64_t rem = n % 1000000ULL;
        if (rem) {
            if (strlen(buf) > 0) strncat(buf, " ", 255);
            words_rec(rem, buf);
        }
    } else if (n >= 1000ULL) {         /* thousands */
        uint64_t thousands = n / 1000ULL;
        words_rec(thousands, buf);
        strncat(buf, " thousand", 255);
        uint64_t rem = n % 1000ULL;
        if (rem) {
            if (strlen(buf) > 0) strncat(buf, " ", 255);
            words_rec(rem, buf);
        }
    } else if (n >= 100ULL) {          /* hundreds */
        uint64_t hundreds = n / 100ULL;
        strncat(buf, under20[hundreds], 255);
        strncat(buf, " hundred", 255);
        uint64_t rem = n % 100ULL;
        if (rem) {
            if (strlen(buf) > 0) strncat(buf, " ", 255);
            words_rec(rem, buf);
        }
    } else if (n >= 20ULL) {           /* tens and units */
        uint64_t t = n / 10;
        strncat(buf, tens[t], 255);
        uint64_t u = n % 10;
        if (u) {
            strncat(buf, "-", 255);
            strncat(buf, under20[u], 255);
        }
    } else {                           /* 0‑19 */
        strncat(buf, under20[n], 255);
    }
}

/* Convert n to its lowercase English word and check if it contains 'v'. */
static bool has_v(uint64_t n)
{
    char buf[256];
    if (n == 0) {
        strcpy(buf, "zero");
    } else {
        buf[0] = '\0';
        words_rec(n, buf);
    }
    /* Search for 'v' (case‑sensitive because all words are lowercase). */
    for (char *p = buf; *p; ++p)
        if (*p == 'v') return true;
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Static cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms cached */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_candidate = 1;  /* next integer to test */

    /* Ensure cache large enough for the required term */
    while (cache_len < n) {
        if (has_v((uint64_t)next_candidate)) {
            if (cache_len == cache_cap) {
                /* Double the capacity (start with 4) */
                grow_seq_array(&cache, &cache_cap, cache_cap ? cache_cap * 2 : 4);
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
