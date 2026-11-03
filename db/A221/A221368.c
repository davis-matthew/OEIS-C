/*
 * A221368  A two-digit Look-and-Say sequence starting with 12: each term
 *          summarizes the increasing two-digit substrings of the previous
 *          term.
 * https://oeis.org/A221368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 945 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A221368: Two‑digit look‑and‑say sequence starting with 12.
   a(0) = 12.
   a(n) (n > 0) is obtained by taking all two‑digit substrings of a(n‑1),
   counting their frequencies, sorting the substrings in increasing order,
   and concatenating for each substring “<count><substring>”. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* Append the decimal representation of an unsigned value
   to *buf, ensuring capacity.  *len and *cap are updated. */
static void append_count(char **buf, size_t *len, size_t *cap, size_t val)
{
    char tmp[32];
    int t = 0;

    /* val > 0 must hold. */
    while (val > 0) {
        tmp[t++] = '0' + (val % 10);
        val /= 10;
    }

    /* Ensure space for the digits plus the two‑digit number. */
    if (*len + (size_t)t + 2 > *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 32;
        while (*len + (size_t)t + 2 > new_cap) new_cap *= 2;
        *buf = (char *)realloc(*buf, new_cap);
        *cap = new_cap;
    }

    /* Copy the digits in correct order. */
    for (int i = t - 1; i >= 0; --i)
        (*buf)[(*len)++] = tmp[i];
}

/* Build the next term from cur (length cur_len) into *next.
   *next_len receives the length (excluding NUL). */
static void produce_next(const char *cur, size_t cur_len,
                         char **next, size_t *next_len, size_t *next_cap)
{
    size_t counts[100] = {0};
    for (size_t i = 0; i + 1 < cur_len; ++i) {
        int v = (cur[i] - '0') * 10 + (cur[i + 1] - '0');
        ++counts[v];
    }

    if (*next == NULL) {
        *next_cap = 128;
        *next = (char *)malloc(*next_cap);
    }

    size_t len = 0;
    for (int v = 0; v < 100; ++v) {
        if (counts[v] == 0) continue;
        append_count(next, &len, next_cap, counts[v]);

        /* Append the two‑digit number. */
        (*next)[len++] = '0' + v / 10;
        (*next)[len++] = '0' + v % 10;
    }
    (*next)[len] = '\0';
    *next_len = len;
}

static seq_t *cache = NULL;          /* memoized terms */
static size_t cache_cap = 0;          /* allocated cache capacity */
static size_t cache_len = 0;          /* number of cached terms */

/* Current term as a NUL‑terminated string. */
static char *cur = NULL;
static size_t cur_len = 0;            /* length without NUL */

/* Temporary buffer to build the next term. */
static char *next_buf = NULL;
static size_t next_cap = 0;

seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute terms up to n. */
    while (cache_len <= (size_t)n) {
        if (cache_len == 0) {
            /* a(0) = 12 */
            cur = (char *)malloc(3);          /* "12" + '\0' */
            strcpy(cur, "12");
            cur_len = 2;

            if (cache_cap == 0) {
                cache_cap = 8;
                cache = mem_new(seq_t, cache_cap);
            }
            cache[cache_len++] = seq_from_str(cur);
        } else {
            size_t new_len;
            produce_next(cur, cur_len, &next_buf, &new_len, &next_cap);

            free(cur);                /* free old string */
            cur = next_buf;           /* new string becomes current */
            cur_len = new_len;
            next_buf = NULL;
            next_cap = 0;

            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                cache = mem_resize(seq_t, cache, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = seq_from_str(cur);  /* cache new value */
        }
    }
    return cache[(size_t)n];
}
