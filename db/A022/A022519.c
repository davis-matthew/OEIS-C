/*
 * A022519  Describe previous term from the right (method B - initial term is
 *          8).
 * https://oeis.org/A022519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   _size_t_to_str, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:   a(0) = 8
 *   a(n) = describe previous term from the right; for a(n‑1) read its
 *   decimal digits from least significant to most significant, group
 *   equal consecutive digits, and for each group output the digit
 *   followed by its run length. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/_size_t_to_str.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;   /* number of cached entries */

    /* ensure capacity for index n */
    if ((size_t)n >= cache_cap) {
        size_t new_lim = cache_cap ? cache_cap * 2 : 1;
        while (new_lim <= (size_t)n) {
            new_lim *= 2;
        }
        seq_t *new_cache = (seq_t*)realloc(cache, new_lim * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_lim;
    }

    /* return cached term if already available */
    if (n < cache_size) {
        return cache[n];
    }

    /* compute missing terms */
    for (idx_t idx = cache_size; idx <= n; ++idx) {
        seq_t val;
        if (idx == 0) {
            val = (seq_t)8;
        } else {
            /* extract decimal digits of previous term from least significant side */
            seq_t tmp = cache[idx - 1];
            size_t digits_cap = 16;
            uint8_t *digits = (uint8_t*)malloc(digits_cap);
            size_t digits_len = 0;
            if (tmp == (seq_t)0) {
                digits[0] = 0;
                digits_len = 1;
            } else {
                while (tmp != (seq_t)0) {
                    uint8_t d = (uint8_t)(tmp % (seq_t)10);
                    if (digits_len == digits_cap) {
                        digits_cap *= 2;
                        digits = (uint8_t*)realloc(digits, digits_cap);
                    }
                    digits[digits_len++] = d;
                    tmp /= (seq_t)10;
                }
            }

            /* build output buffer */
            size_t out_cap = digits_len * 2 + 16;
            unsigned char *outbuf = (unsigned char*)malloc(out_cap);
            size_t out_len = 0;

            uint8_t run_digit = digits[0];
            size_t run_len = 1;
            for (size_t i = 1; i < digits_len; ++i) {
                if (digits[i] == run_digit) {
                    ++run_len;
                } else {
                    /* append digit */
                    if (out_len == out_cap) {
                        out_cap *= 2;
                        outbuf = (unsigned char*)realloc(outbuf, out_cap);
                    }
                    outbuf[out_len++] = '0' + run_digit;
                    /* append run length */
                    unsigned char tmp_buf[32];
                    size_t tmp_len;
                    _size_t_to_str(run_len, tmp_buf, &tmp_len);
                    for (size_t k = 0; k < tmp_len; ++k) {
                        if (out_len == out_cap) {
                            out_cap *= 2;
                            outbuf = (unsigned char*)realloc(outbuf, out_cap);
                        }
                        outbuf[out_len++] = tmp_buf[k];
                    }
                    /* reset */
                    run_digit = digits[i];
                    run_len = 1;
                }
            }
            /* last run */
            if (out_len == out_cap) {
                out_cap *= 2;
                outbuf = (unsigned char*)realloc(outbuf, out_cap);
            }
            outbuf[out_len++] = '0' + run_digit;
            unsigned char tmp_buf[32];
            size_t tmp_len;
            _size_t_to_str(run_len, tmp_buf, &tmp_len);
            for (size_t k = 0; k < tmp_len; ++k) {
                if (out_len == out_cap) {
                    out_cap *= 2;
                    outbuf = (unsigned char*)realloc(outbuf, out_cap);
                }
                outbuf[out_len++] = tmp_buf[k];
            }

            /* convert to seq_t */
            char *cstr = (char*)malloc(out_len + 1);
            for (size_t i = 0; i < out_len; ++i) {
                cstr[i] = (char)outbuf[i];
            }
            cstr[out_len] = '\0';
            val = seq_from_str(cstr);

            free(cstr);
            free(outbuf);
            free(digits);
        }
        cache[idx] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
