/*
 * A160661  Numbers that have a "t" in the middle of their names.
 * https://oeis.org/A160661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers with a “t” in the middle of their spelled‑out English name.
   This function computes the sequence by searching for candidates and
   caching the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility functions for dynamic handling of seq_t. */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* English name components. */
static const char *ONES[10] = {
    "zero","one","two","three","four","five","six","seven","eight","nine"
};
static const char *TEENS[10] = {
    "ten","eleven","twelve","thirteen","fourteen",
    "fifteen","sixteen","seventeen","eighteen","nineteen"
};
static const char *TENS[10] = {
    "", "", "twenty","thirty","forty","fifty",
    "sixty","seventy","eighty","ninety"
};
static const char *GROUPS[] = {
    "", "thousand", "million", "billion", "trillion"
};

/* Append a null‑terminated string to a buffer, updating its length. */
static void append(char *buf, int *len, const char *s)
{
    for (; *s; ++s) {
        buf[(*len)++] = *s;
    }
}

/* Convert a number 0 < n < 1000 into words and append to buf. */
static void convert_group(uint16_t n, char *buf, int *len)
{
    uint16_t h = n / 100;
    uint16_t rem = n % 100;
    if (h) {
        append(buf, len, ONES[h]);
        append(buf, len, "hundred");
    }
    if (rem) {
        if (rem < 10) {
            append(buf, len, ONES[rem]);
        } else if (rem < 20) {
            append(buf, len, TEENS[rem - 10]);
        } else {
            uint16_t t = rem / 10;
            uint16_t o = rem % 10;
            append(buf, len, TENS[t]);
            if (o) {
                append(buf, len, ONES[o]);
            }
        }
    }
}

/* Build the concatenated English name of n (without spaces or “and”) into buf.
   Returns the length of the resulting string. */
static int number_name(uint64_t n, char *buf)
{
    int len = 0;
    if (n == 0) {
        append(buf, &len, ONES[0]);
        return len;
    }
    /* Break into groups of 3 digits. */
    uint16_t groups[10];
    int gcnt = 0;
    while (n) {
        groups[gcnt++] = (uint16_t)(n % 1000);
        n /= 1000;
    }
    /* Process from most significant group. */
    for (int i = gcnt - 1; i >= 0; --i) {
        if (groups[i]) {
            convert_group(groups[i], buf, &len);
            if (i > 0) {
                append(buf, &len, GROUPS[i]);
            }
        }
    }
    return len;
}

/* Test whether a number satisfies the OEIS condition. */
static bool ok_number(uint64_t n)
{
    char buf[512];
    int len = number_name(n, buf);
    if ((len & 1) == 0) return false;
    return buf[len / 2] == 't';
}

/* Cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 0;   /* last candidate examined */

/* Ensure cache has space for at least n elements. */
static void ensure_cache(void)
{
    if (cache_size + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) {
            /* Allocation failed – abort. */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour below is irrelevant */
    size_t target = (size_t)n;
    /* Extend cache up to n terms. */
    while (cache_size < target) {
        for (uint64_t cand = last_checked + 1; ; ++cand) {
            if (ok_number(cand)) {
                ensure_cache();
                cache[cache_size++] = (seq_t)cand;
                last_checked = cand;
                if (cache_size == target) return cache[cache_size - 1];
                break;
            }
            last_checked = cand;
        }
    }
    return cache[target - 1];
}
