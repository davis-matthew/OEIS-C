/*
 * A036106  A summarize Fibonacci sequence starting with a(0)=1 and a(1)=2:
 *          summarize the previous two terms!.
 * https://oeis.org/A036106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   a(n) = concatenation of (count, digit) pairs, sorted by digit
 *            descending, where count is the total number of occurrences of
 *            digit in a(n‑2) and a(n‑1). a(0) = 1, a(1) = 2. The digits of
 *            each term are treated as decimal digits
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 254, u128 n <= 254,
 *            big any n
 * Helpers:   digit_len, num_digits_uint, seq_from_str, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "../utils/seq_from_str.h"
#include "../utils/seq_to_str.h"
#include "../utils/digit_len.h"
#include "../utils/num_digits_uint.h"

/* Helper: append an unsigned integer to a character buffer */
static void append_uint(char **p, unsigned int v)
{
    char tmp[20];
    int len = 0;
    unsigned int t = v;
    do { tmp[len++] = '0' + (t % 10); t /= 10; } while (t);
    char *ptr = *p;
    for (int i = len - 1; i >= 0; --i) *ptr++ = tmp[i];
    *p = ptr;
}

/* The main function requested by the challenge */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;

    /* One‑time cache initialization */
    if (!cache) {
        cap = 4;                      /* start with a small capacity */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = seq_from_str("1");
        cache[1] = seq_from_str("2");
        len = 2;
    }

    /* Expand cache if needed */
    if (len <= n) {
        size_t required = n + 1;
        if (required > cap) {
            size_t new_cap = cap ? cap * 2 : 4;
            while (new_cap < required) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cap = new_cap;
        }

        /* Compute missing terms up to n */
        for (idx_t i = (idx_t)len; i <= n; ++i) {
            unsigned int cnt[10] = {0};

            /* Count digits in a(i-2) */
            {
                size_t l = digit_len(cache[i-2]);
                char *s = (char *)malloc(l + 1);
                size_t rl = l;
                seq_to_str(cache[i-2], s, &rl);
                s[rl] = '\0';
                for (size_t k = 0; k < rl; ++k) ++cnt[s[k] - '0'];
                free(s);
            }

            /* Count digits in a(i-1) */
            {
                size_t l = digit_len(cache[i-1]);
                char *s = (char *)malloc(l + 1);
                size_t rl = l;
                seq_to_str(cache[i-1], s, &rl);
                s[rl] = '\0';
                for (size_t k = 0; k < rl; ++k) ++cnt[s[k] - '0'];
                free(s);
            }

            /* Estimate length of the new string */
            size_t est = 0;
            for (int d = 9; d >= 0; --d) {
                if (cnt[d]) est += num_digits_uint(cnt[d]) + 1;
            }

            char *buf = (char *)malloc(est + 1);
            char *p = buf;
            for (int d = 9; d >= 0; --d) {
                if (cnt[d]) {
                    append_uint(&p, cnt[d]);
                    *p++ = '0' + d;
                }
            }
            *p = '\0';

            /* Convert to seq_t and store */
            cache[i] = seq_from_str(buf);
            free(buf);
        }
        len = n + 1;
    }

    return cache[n];
}
