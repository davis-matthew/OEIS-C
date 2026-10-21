/*
 * A093277  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 77.
 * https://oeis.org/A093277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   Piecewise definition of a(n) – the largest integer whose all
 *            length‑n substrings are distinct and divisible by 77. For n = 1,
 *            2, 3: 0, 77, 924. For even n: if n ≡ 0 (mod 6) : a(n) = 9^(n‑2)
 *            22 9^(n‑2) 2 else : a(n) = 7^(n‑2) 00 7^(n‑2) 0 For odd n
 *            divisible by 3: a(n) = 9^(n‑3) 924 For odd n ≡ 1...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   fill_chars, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"
#include "../utils/fill_chars.h"

/* Main sequence function */
seq_t a(idx_t n) {
    /* Handlers for the very small indices */
    if (n == 1) return seq_from_str("0");
    if (n == 2) return seq_from_str("77");
    if (n == 3) return seq_from_str("924");

    /* Even n */
    if ((n & 1) == 0) {
        /* n >= 4 in this branch */
        size_t repeat = (size_t)(n - 2);          /* (n-2) repetitions of one digit */
        size_t total_len = repeat + 2 + repeat + 1; /* middle two digits + tail */
        char *buf = (char *)malloc(total_len + 1); /* +1 for NUL */

        if (n % 6 == 0) {
            /* 9^(n-2) 22 9^(n-2) 2 */
            fill_chars(buf, repeat, '9');
            buf[repeat]     = '2';
            buf[repeat + 1] = '2';
            fill_chars(buf + repeat + 2, repeat, '9');
            buf[repeat + 2 + repeat] = '2';
        } else {
            /* 7^(n-2) 00 7^(n-2) 0 */
            fill_chars(buf, repeat, '7');
            buf[repeat]     = '0';
            buf[repeat + 1] = '0';
            fill_chars(buf + repeat + 2, repeat, '7');
            buf[repeat + 2 + repeat] = '0';
        }
        buf[total_len] = '\0';
        seq_t val = seq_from_str(buf);
        free(buf);
        return val;
    }

    /* Odd n */
    /* n ≥ 5 here, because n=1,3 were handled before */
    if (n % 3 == 0) {
        /* 9^(n-3) 924 */
        size_t nine_count = (size_t)(n - 3);
        size_t total_len = nine_count + 3; /* digits of 924 */
        char *buf = (char *)malloc(total_len + 1);
        fill_chars(buf, nine_count, '9');
        buf[nine_count]     = '9';
        buf[nine_count + 1] = '2';
        buf[nine_count + 2] = '4';
        buf[total_len]      = '\0';
        seq_t val = seq_from_str(buf);
        free(buf);
        return val;
    }

    if (n % 6 == 1) {
        /* 8^(n-1) 0 3^(n-1) 0 */
        size_t repeat = (size_t)(n - 1);
        size_t total_len = repeat + 1 + repeat + 1;
        char *buf = (char *)malloc(total_len + 1);
        fill_chars(buf, repeat, '8');
        buf[repeat] = '0';
        fill_chars(buf + repeat + 1, repeat, '3');
        buf[total_len - 1] = '0';
        buf[total_len] = '\0';
        seq_t val = seq_from_str(buf);
        free(buf);
        return val;
    }

    /* n % 6 == 5 */
    /* 3^(n-3) 0 33 8^(n-3) 0 88 */
    size_t part = (size_t)(n - 3);
    size_t total_len = part + 1 + 2 + part + 1 + 2;
    char *buf = (char *)malloc(total_len + 1);
    size_t pos = 0;
    /* 3^(n-3) */
    fill_chars(buf, part, '3');
    pos += part;
    buf[pos++] = '0';
    buf[pos++] = '3';
    buf[pos++] = '3';
    /* 8^(n-3) */
    fill_chars(buf + pos, part, '8');
    pos += part;
    buf[pos++] = '0';
    buf[pos++] = '8';
    buf[pos++] = '8';
    buf[pos] = '\0';
    seq_t val = seq_from_str(buf);
    free(buf);
    return val;
}
