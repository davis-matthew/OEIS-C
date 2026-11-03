/*
 * A228893  Numbers k such that the concatenation of the frequency of each
 *          decimal digit of k is divisible by k.
 * https://oeis.org/A228893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 68 terms through n = 79 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A228893: Numbers k such that the concatenation of the
   frequencies of each decimal digit of k is divisible by k.
   Computed by brute‑force enumeration of k = 1,2,3,… and selecting
   those that satisfy the property. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* convert an unsigned integer to its decimal representation in buf.
   buf is filled with the characters '0'..'9' and the function returns
   the number of characters written. */
static int digits_uint(unsigned int x, unsigned char *buf)
{
    int len = 0;
    if (x == 0) {
        buf[0] = '0';
        return 1;
    }
    while (x > 0) {
        buf[len++] = (unsigned char)('0' + (x % 10));
        x /= 10;
    }
    /* reverse to get the most significant digit first */
    for (int i = 0; i < len / 2; ++i) {
        unsigned char t = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = t;
    }
    return len;
}

/* test whether k satisfies the defining property:
   the concatenated frequency string is divisible by k. */
static bool good_k(seq_t k)
{
    unsigned int freq[10] = {0};

    seq_t t = k;
    if (t == 0) {
        freq[0] = 1;              /* k == 0 never occurs, but keep for safety */
    } else {
        while (t != 0) {
            unsigned int d = (unsigned int)(t % 10);
            ++freq[d];
            t /= 10;
        }
    }

    seq_t rem = 0;
    unsigned char buf[20];
    for (int d = 0; d < 10; ++d) {
        unsigned int f = freq[d];
        int len = digits_uint(f, buf);
        for (int i = 0; i < len; ++i) {
            rem = (rem * (seq_t)10 + (seq_t)(buf[i] - '0')) % k;
        }
    }
    return rem == 0;
}

/* dynamic cache of found terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;          /* next integer to test */

static void ensure_cache_capacity(size_t nh)
{
    if (cache_cap < nh) {
        size_t new_cap = cache_cap > 0 ? cache_cap << 1 : 8;
        while (new_cap < nh) new_cap <<= 1;
        seq_t *p = mem_resize(seq_t, cache, new_cap);
        if (!p) abort();
        cache = p;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n is 1‑based; guard against zero */

    while (cache_len < (size_t)n) {
        seq_t k = (seq_t)next_k;
        if (good_k(k)) {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
