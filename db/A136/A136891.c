/*
 * A136891  Numbers k such that k and k^2 use only the digits 0, 2, 3, 5 and
 *          9.
 * https://oeis.org/A136891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 71, u32 n <= 965, u64 n <= 1374,
 *            u128 n <= 1374, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the sequence of numbers k such that k
   and k^2 use only the digits 0,2,3,5,9 (OEIS A136891). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

/* cache of already-found terms */
static seq_t *seq      = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;
static int    max_len  = 0;

/* ensure the storage array is large enough */
static void ensure_capacity(void)
{
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap > 0) ? seq_cap * 2 : 4;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* test if a decimal digit is allowed */
static inline bool allowed_digit(uint8_t d)
{
    return d == 0 || d == 2 || d == 3 || d == 5 || d == 9;
}

/* check whether the square of the number represented by
   `digs[0..len-1]` (most significant first) contains only allowed
   digits. */
static bool square_allowed(const uint8_t *digs, size_t len)
{
    /* reverse to little‑endian for multiplication */
    uint8_t *low = (uint8_t *)malloc(len);
    for (size_t i = 0; i < len; ++i) low[i] = digs[len - 1 - i];

    /* convolution of the digits */
    size_t res_len = 2 * len;
    uint32_t *res = (uint32_t *)calloc(res_len + 1, sizeof(uint32_t));
    for (size_t i = 0; i < len; ++i)
        for (size_t j = 0; j < len; ++j)
            res[i + j] += low[i] * low[j];

    /* propagate carries in base 10 */
    for (size_t i = 0; i < res_len; ++i) {
        uint32_t carry = res[i] / 10;
        res[i] %= 10;
        res[i + 1] += carry;
    }

    /* locate the most significant non‑zero digit */
    int hi = (int)res_len;
    while (hi > 0 && res[hi] == 0) hi--;

    /* check digits */
    for (int i = 0; i <= hi; ++i)
        if (!allowed_digit((uint8_t)res[i])) {
            free(low);
            free(res);
            return false;
        }

    free(low);
    free(res);
    return true;
}

/* recursive generation of all numbers of a fixed length
   pos   : current position (0‑based, already set)
   len   : total length
   buf   : array of digits (most significant first)
   val   : accumulated numeric value in unsigned __int128          */
static void gen_rec(int pos, int len, uint8_t *buf,
                    unsigned __int128 val)
{
    static const uint8_t digits[5] = {0, 2, 3, 5, 9};

    if (pos == len) {
        if (!square_allowed(buf, (size_t)len)) return;

        /* Does the number fit into seq_t? */
        bool fits = true;
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 maxv = (unsigned __int128)(((seq_t)(-1)));
            fits = val <= maxv;
        }

        if (!fits) return;

        ensure_capacity();
        if (sizeof(seq_t) <= 8) {
            seq[seq_len++] = (seq_t)val;
        } else {
            /* convert to decimal string */
            char s[256];
            unsigned __int128 tmp = val;
            int idx = 0;
            if (tmp == 0) {
                s[idx++] = '0';
            } else {
                while (tmp) {
                    s[idx++] = '0' + (tmp % 10);
                    tmp /= 10;
                }
                for (int l = 0, r = idx - 1; l < r; ++l, --r) {
                    char c = s[l]; s[l] = s[r]; s[r] = c;
                }
            }
            s[idx] = '\0';
            seq[seq_len++] = seq_from_str(s);
        }
        return;
    }

    /* prune when we already exceed maximum value for fixed‑size types */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 maxv = (unsigned __int128)(((seq_t)(-1)));
        if (val > maxv) return;
    }

    for (size_t i = 0; i < 5; ++i) {
        buf[pos] = digits[i];
        unsigned __int128 new_val = val * 10 + digits[i];
        gen_rec(pos + 1, len, buf, new_val);
    }
}

/* generate all numbers of a given length (len >= 1) */
static void generate_length(int len)
{
    static const uint8_t first[4] = {2, 3, 5, 9};
    uint8_t buf[64];
    for (size_t i = 0; i < 4; ++i) {
        buf[0] = first[i];
        gen_rec(1, len, buf, first[i]);
    }
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, harmless */

    /* insert the term 0 (the first element) if not present */
    if (seq_len == 0) {
        ensure_capacity();
        seq[seq_len++] = (seq_t)0;
    }

    while ((size_t)n > seq_len) {
        ++max_len;
        generate_length(max_len);
    }
    return seq[(size_t)n - 1];
}
