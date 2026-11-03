/*
 * A022513  Describe previous term from the right (method A - initial term is
 *          9).
 * https://oeis.org/A022513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(0) = 9; For n ≥ 0, a(n+1) is obtained by scanning a(n) from
 *            right to left, grouping equal consecutive digits; for each group
 *            append the decimal count followed by the digit
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-03)
 * Helpers:   bit_length, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/bit_length.h"
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

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

seq_t a(idx_t n)
{
    /* Cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;  /* allocated cache size */
    static idx_t  cache_max  = (idx_t)-1;  /* largest index computed */

    /* Initialize cache on first call */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache     = mem_new(seq_t, 1);
        cache[0]  = (seq_t)9;            /* a(0) = 9 */
        cache_max = 0;
    }

    /* Return cached value if already computed */
    if (n <= cache_max)
        return cache[n];

    /* Ensure cache array large enough for n+1 entries */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t idx = cache_max; idx < n; ++idx) {
        seq_t prev = cache[idx];

        /* Buffer for string representation of prev */
        size_t bitlen = bit_length(prev);
        size_t est_digits = (bitlen * 301 + 99) / 100 + 2;  /* safety margin */
        char *buf = (char*)malloc(est_digits + 1);
        size_t len = 0;
        seq_to_str(prev, buf, &len);
        buf[len] = '\0';   /* ensure NUL terminator */

        /* Build next term by scanning right to left */
        size_t out_cap = 2 * len + 10;               /* generous initial size */
        char *out_buf = (char*)malloc(out_cap);
        size_t out_len = 0;

        size_t i = len;
        while (i > 0) {
            char digit = buf[--i];
            size_t count = 1;
            while (i > 0 && buf[i-1] == digit) {
                ++count;
                --i;
            }
            /* Convert count to decimal digits (stored reversed) */
            char cnt_buf[32];
            size_t cnt_len = 0;
            size_t tmp = count;
            do {
                cnt_buf[cnt_len++] = '0' + (tmp % 10);
                tmp /= 10;
            } while (tmp);
            /* Append count digits in correct order */
            for (size_t j = cnt_len; j-- > 0; )
                out_buf[out_len++] = cnt_buf[j];
            /* Append the digit */
            out_buf[out_len++] = digit;
        }
        out_buf[out_len] = '\0';

        /* Convert new string to seq_t and store */
        cache[idx + 1] = seq_from_str(out_buf);

        free(buf);
        free(out_buf);
    }

    cache_max = n;
    return cache[n];
}
