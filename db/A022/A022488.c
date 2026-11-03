/*
 * A022488  Describe previous term from the right (method B - initial term is
 *          2).
 * https://oeis.org/A022488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(1)=2; for n>1 a(n) is obtained by describing the terms of a(n-1)
 * from the right: group consecutive equal digits, and for each group write
 * the digit followed by the run length. This is method B in OEIS A022488. */

#include <stdlib.h>
#include <stddef.h>
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

/* helper: compute the next term string from prev string (len).  The caller
   receives a newly allocated null‑terminated string and the length (excluding
   the terminator).  The string must be freed by the caller. */
static char *next_string(const char *prev, size_t len, size_t *out_len)
{
    size_t cap = 64;
    char *buf = (char *)malloc(cap);
    size_t pos = 0;
    for (ptrdiff_t j = (ptrdiff_t)len; j > 0; ) {
        char digit = prev[j-1];
        size_t cnt = 1;
        j--;
        while (j > 0 && prev[j-1] == digit) {
            cnt++;
            j--;
        }
        /* count digits of cnt */
        size_t cnt_len = 0;
        size_t tmp_val = cnt;
        do { cnt_len++; tmp_val /= 10; } while (tmp_val > 0);
        /* ensure capacity */
        if (pos + 1 + cnt_len >= cap) {
            cap = cap * 2 + 1 + cnt_len;
            buf = (char *)realloc(buf, cap);
        }
        buf[pos++] = digit;
        /* write cnt in decimal */
        char tmp[32];
        size_t tmp_pos = 0;
        size_t t = cnt;
        do { tmp[tmp_pos++] = (char)('0' + (t % 10)); t /= 10; } while (t > 0);
        for (size_t k = tmp_pos; k > 0; --k)
            buf[pos++] = tmp[k-1];
    }
    buf[pos] = '\0';
    *out_len = pos;
    return buf;
}

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t current_max = 0;
static char *last_str = NULL;
static size_t last_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but keep safe */

    /* initialize the first term if not yet done */
    if (current_max == 0) {
        /* a(1) = 2 */
        seq_t v = 2;
        if (cache_cap == 0) {
            cache_cap = 8;
            cache = mem_new(seq_t, cache_cap);
        }
        cache[0] = v;
        cache_len = 1;
        current_max = 1;
        last_len = 1;
        last_str = (char *)malloc(2);
        last_str[0] = '2';
        last_str[1] = '\0';
    }

    /* compute missing terms up to n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        size_t new_len;
        char *new_str = next_string(last_str, last_len, &new_len);
        seq_t val = seq_from_str(new_str);
        /* store in cache */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2 + 8;
            cache = mem_resize(seq_t, cache, cache_cap);
        }
        cache[cache_len++] = val;
        /* free the old string and prepare for next iteration */
        free(last_str);
        last_str = new_str;
        last_len = new_len;
        current_max = i;
    }

    return cache[n - 1];
}
