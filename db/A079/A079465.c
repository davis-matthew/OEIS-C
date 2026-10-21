/*
 * A079465  Numbers k such that the "inventory" A063850 of k is a perfect
 *          square.
 * https://oeis.org/A079465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 12, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A079465: numbers whose decimal inventory is a perfect square */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Compute the inventory of a number: for each distinct digit in the
   order of first appearance (left‑to‑right in decimal), append the count
   of that digit followed by the digit itself. */
static seq_t inventory(seq_t k)
{
    /* arrays for digit counts and first‑appearance order */
    unsigned int  cnt[10] = {0};
    unsigned char order[10];
    unsigned char seen[10] = {0};
    size_t order_len = 0;
    /* buffer to hold the digits of k in reverse order */
    unsigned char rev[32];
    size_t rev_len = 0;
    seq_t tmp = k;

    /* handle zero explicitly */
    if (tmp == 0) {
        rev[rev_len++] = 0;
    } else {
        while (tmp > 0) {
            rev[rev_len++] = (unsigned char)(tmp % 10);
            tmp /= 10;
        }
    }

    /* iterate from most significant digit to least to determine order,
       while counting frequencies */
    for (size_t i = 0; i < rev_len; ++i) {
        unsigned char digit = rev[rev_len - 1 - i];
        if (!seen[digit]) {
            seen[digit] = 1;
            order[order_len++] = digit;
        }
        ++cnt[digit];
    }

    /* build the inventory number */
    seq_t inv = 0;
    for (size_t i = 0; i < order_len; ++i) {
        unsigned int digit = order[i];
        unsigned int c     = cnt[digit];

        /* length of decimal representation of c */
        unsigned int c_len = 0;
        unsigned int tmp_c = c;
        do {
            ++c_len;
            tmp_c /= 10;
        } while (tmp_c > 0);

        /* 10^c_len */
        seq_t pow10 = 1;
        for (unsigned int j = 0; j < c_len; ++j)
            pow10 *= 10;

        inv = inv * pow10 + c;   /* append count    */
        inv = inv * 10 + digit;  /* append digit    */
    }
    return inv;
}

/* Test whether a sequence value is a perfect square */
static bool is_square(seq_t v)
{
    seq_t s = floor_sqrt(v);
    return s * s == v;
}

/* Main function to return the nth term of the sequence */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t last_checked = 0;  /* maximum k tested so far */

    /* grow cache as needed */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        seq_t k = last_checked + 1;
        /* search for the next qualifying k */
        for (;; ++k) {
            seq_t inv = inventory(k);
            if (is_square(inv)) {
                /* add to cache */
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (tmp == NULL) {
                        free(cache);
                        exit(1);
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                last_checked = k;
                break;
            }
        }
    }
    return cache[n - 1];
}
