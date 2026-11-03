/*
 * A049192  Start with 1. Read as base 10, convert to base 2, describe it in
 *          base 2.
 * https://oeis.org/A049192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the binary look‑and‑say sequence starting with 1. a(1) =
 *            1 For n > 1: read a(n‑1) as a decimal integer, convert it to
 *            binary, then run‑length encode that binary string, writing
 *            counts (in binary) followed by the bit value, and use the
 *            resulting bit string as the next decimal number
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static char *mkstr(const char *s) {
    size_t len = strlen(s);
    char *p = (char *)malloc(len + 1);
    memcpy(p, s, len + 1);
    return p;
}

/* Convert a decimal string to its binary representation (no leading zeros). */
static char *decimal_to_binary(const char *dec) {
    if (dec[0] == '0' && dec[1] == '\0')
        return mkstr("0");

    /* Duplicate input string to modify it. */
    char *cur = mkstr(dec);
    size_t cur_len = strlen(cur);

    /* Store bits in reverse order. */
    size_t bits_cap = 8;
    char *bits_rev = (char *)malloc(bits_cap);
    size_t bits_len = 0;

    while (!(cur_len == 1 && cur[0] == '0')) {
        int carry = 0;
        /* Allocate quotient string. */
        char *quot = (char *)malloc(cur_len + 1);
        size_t w = 0;
        for (size_t i = 0; i < cur_len; ++i) {
            int digit = cur[i] - '0';
            int num   = carry * 10 + digit;
            int q     = num / 2;
            int r     = num % 2;
            if (w > 0 || q > 0) {
                quot[w++] = '0' + q;
            }
            carry = r;
        }
        if (w == 0) {                /* quotient is 0 */
            quot[0] = '0';
            w = 1;
        }
        quot[w] = '\0';

        /* Store the remainder bit. */
        if (bits_len == bits_cap) {
            bits_cap <<= 1;
            bits_rev = (char *)realloc(bits_rev, bits_cap);
        }
        bits_rev[bits_len++] = '0' + carry;

        free(cur);
        cur = quot;
        cur_len = w;
    }

    /* Build binary string from reversed bits. */
    char *bin = (char *)malloc(bits_len + 1);
    for (size_t i = 0; i < bits_len; ++i)
        bin[i] = bits_rev[bits_len - 1 - i];
    bin[bits_len] = '\0';

    free(bits_rev);
    free(cur);          /* cur is the string "0" */
    return bin;
}

/* Run‑length encode a binary string, writing counts (in binary) followed by the bit value. */
static char *run_length_encode(const char *bin) {
    size_t L = strlen(bin);

    size_t out_cap = L * 2 + 10;
    char *out = (char *)malloc(out_cap);
    size_t out_len = 0;

    size_t i = 0;
    while (i < L) {
        char bit = bin[i];
        size_t j = i + 1;
        while (j < L && bin[j] == bit) ++j;
        size_t count = j - i;

        /* Convert count to binary string (no leading zeros). */
        unsigned long long c = count;
        char tmp_bin[64];
        int tmp_len = 0;
        while (c > 0) {
            tmp_bin[tmp_len++] = (c & 1) ? '1' : '0';
            c >>= 1;
        }
        /* Append the count bits in correct order. */
        for (int k = tmp_len - 1; k >= 0; --k) {
            if (out_len == out_cap) {
                out_cap <<= 1;
                out = (char *)realloc(out, out_cap);
            }
            out[out_len++] = tmp_bin[k];
        }
        /* Append the bit value. */
        if (out_len == out_cap) {
            out_cap <<= 1;
            out = (char *)realloc(out, out_cap);
        }
        out[out_len++] = bit;

        i = j;
    }

    out[out_len] = '\0';
    return out;
}

/* Cache of computed terms (stored as decimal strings). */
static char **cached_terms = NULL;
static size_t cached_size  = 0;

/* Main sequence function. */
seq_t a(idx_t n) {
    if (!cached_terms) {
        /* Allocate room for index 1. */
        cached_size = 2;
        cached_terms = (char **)malloc(sizeof(char *) * cached_size);
        cached_terms[1] = mkstr("1");
    }

    if ((size_t)n >= cached_size) {
        /* Expand cache. */
        cached_size = (size_t)n + 1;
        cached_terms = (char **)realloc(cached_terms, sizeof(char *) * cached_size);
        for (size_t i = 2; i <= n; ++i) {
            char *bin = decimal_to_binary(cached_terms[i - 1]);
            char *next = run_length_encode(bin);
            free(bin);
            cached_terms[i] = next;
        }
    }

    return seq_from_str(cached_terms[n]);
}
