/*
 * A181720  a(n) = smallest multiple of n which contains its base 10 reversal
 *          as a substring.
 * https://oeis.org/A181720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest multiple of n that contains reverse(n) as a
 *            substring
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   reverse_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "../utils/reverse_str.h"

/* convert a seq_t value to a decimal string */
static size_t seq_to_string(seq_t x, char *buf, size_t bufsize)
{
    /* handle zero explicitly */
    if (x == 0) {
        if (bufsize < 2) return 0;
        buf[0] = '0';
        buf[1] = '\0';
        return 1;
    }

    /* temporary buffer for reversed digits */
    char tmp[2048];
    size_t pos = 0;

    seq_t v = x;
    while (v != 0) {
        seq_t q = v / 10;
        seq_t r = v - q * 10;           /* remainder (v % 10) */
        tmp[pos++] = '0' + (unsigned int)r;
        v = q;
    }

    if (pos == 0) pos = 1; /* safety */

    /* copy digits in correct order */
    size_t i;
    size_t out = (pos < bufsize - 1) ? pos : (bufsize - 1);
    for (i = 0; i < out; ++i) {
        buf[i] = tmp[out - 1 - i];
    }
    if (bufsize > out) buf[out] = '\0';
    return out;
}

/* main function requested */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* undefined for n==0, return 0 to avoid UB */
        return (seq_t)0;
    }

    /* convert n to decimal string */
    char n_str[128];
    size_t n_len = seq_to_string((seq_t)n, n_str, sizeof(n_str));

    /* create pattern: reverse of n_str */
    char pattern[128];
    memcpy(pattern, n_str, n_len);
    pattern[n_len] = '\0';
    reverse_str(pattern, n_len);

    /* buffer for candidate multiple m */
    char m_str[2048];
    seq_t m;
    idx_t k = 1;

    for (;;) {
        /* compute m = n * k */
        m = (seq_t)n * (seq_t)k;

        /* convert m to string */
        size_t m_len = seq_to_string(m, m_str, sizeof(m_str));

        /* check if pattern is a substring of m_str */
        if (strstr(m_str, pattern) != NULL) {
            return m;
        }
        ++k;
    }
}
