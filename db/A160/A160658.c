/*
 * A160658  Numbers that have the letter "n" in the middle of their names when
 *          written in American English.
 * https://oeis.org/A160658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose American English name has an 'n' in the exact
   middle position of the concatenated, lowercase word (no spaces). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ----- English words tables ----- */
static const char *const UNITS[10] = {
    "zero","one","two","three","four","five","six","seven","eight","nine"
};
static const char *const TEENS[10] = {
    "ten","eleven","twelve","thirteen","fourteen",
    "fifteen","sixteen","seventeen","eighteen","nineteen"
};
static const char *const TENS[8] = {
    "twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"
};
static const char *const SCALE[8] = {
    "thousand","million","billion","trillion",
    "quadrillion","quintillion","sextillion","septillion"
};

/* Append string *s* to *buf* at position *len* */
static void append_str(char *buf, size_t *len, const char *s)
{
    size_t l = strlen(s);
    memcpy(buf + *len, s, l);
    *len += l;
}

/* Recursive helper that writes the English word of *n* (n>0) into *buf* */
static void write_words(uint64_t n, char *buf, size_t *len)
{
    if (n < 20) {
        if (n < 10)
            append_str(buf, len, UNITS[n]);
        else
            append_str(buf, len, TEENS[n - 10]);
        return;
    }

    if (n < 100) {
        append_str(buf, len, TENS[n / 10 - 2]);
        if (n % 10)
            append_str(buf, len, UNITS[n % 10]);
        return;
    }

    if (n < 1000) {
        append_str(buf, len, UNITS[n / 100]);
        append_str(buf, len, "hundred");
        if (n % 100)
            write_words(n % 100, buf, len);
        return;
    }

    /* n >= 1000: find the largest power of 1000 that fits */
    uint64_t div = 1000;
    size_t idx = 0;
    while (n / div >= 1000) {
        div *= 1000;
        ++idx;
    }
    uint64_t high = n / div;
    uint64_t low  = n % div;

    write_words(high, buf, len);
    append_str(buf, len, SCALE[idx]);

    if (low)
        write_words(low, buf, len);
}

/* Returns the length of the English name of *n* (without null terminator). */
/* The buffer *buf* is filled with the word. */
static size_t name_length(uint64_t n, char *buf)
{
    size_t len = 0;
    write_words(n, buf, &len);
    buf[len] = '\0';
    return len;
}

/* Test whether *n* has an 'n' exactly at the middle of its English name. */
static bool n_in_middle(uint64_t n)
{
    char buf[512];                /* enough for all needed numbers */
    size_t len = name_length(n, buf);
    return (len & 1) && buf[len / 2] == 'n';
}

/* ----- Cached sequence storage ----- */
static seq_t *cache      = NULL;   /* array of terms */
static idx_t cache_size  = 0;       /* number of stored terms */
static idx_t cache_cap   = 0;       /* allocated capacity */
static uint64_t search_start = 1;   /* next integer to test */

/* Ensure storage can hold at least *need* elements. */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = (size_t)need * 2;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for offset 1 */

    if (n > cache_size) {
        ensure_cache(n);
        while (cache_size < n) {
            uint64_t val = search_start;
            while (!n_in_middle(val))
                ++val;
            cache[cache_size] = (seq_t)val;
            ++cache_size;
            search_start = val + 1;
        }
    }
    return cache[n - 1];
}
