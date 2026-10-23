/*
 * A043022  Base-8 palindromes that start with 2.
 * https://oeis.org/A043022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 8*A043066(n)-6, i.e., base‑8 palindromes that start with
 *            the digit 2
 * Range:     u8 n <= 10, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow8_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow8_uint64.h"

/* Count how many base‑8 palindromes starting with 2 of a given length. */
static idx_t count_len(idx_t len) {
    if (len == 1) return (idx_t)1;          /* only "2" */
    idx_t half = len / 2;
    if (len % 2 == 0)                       /* even length */
        return pow8_uint64(half - 1);
    else                                    /* odd length */
        return pow8_uint64(half);           /* half == floor(len/2) */
}

/* Build the n‑th term (1‑based).  */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined, but safe */

    idx_t rem = n;
    idx_t len = 1;
    while (1) {
        idx_t cnt = count_len(len);
        if (rem <= cnt) break;
        rem -= cnt;
        ++len;
    }

    /* Special case: length 1.  */
    if (len == 1)
        return (seq_t)2;

    idx_t k = rem - 1;          /* zero‑based index within this length block */

    /* Prepare digit array.  */
    uint8_t digits[64] = {0};
    digits[0] = 2;
    digits[len - 1] = 2;

    int half = (int)(len / 2);                /* half = floor(len/2) */
    int odd  = (len % 2 != 0);

    if (odd) {                                 /* odd length */
        int free = half;                       /* count of free digits */
        int fd[64] = {0};
        idx_t t = k;
        for (int i = free - 1; i >= 0; --i) {  /* base‑8 representation */
            fd[i] = (int)(t % 8);
            t /= 8;
        }
        int pos = 1;
        for (int i = 0; i < half - 1; ++i)    /* digits after the leading 2 */
            digits[pos++] = (uint8_t)fd[i];
        digits[half] = (uint8_t)fd[half - 1];  /* middle digit */

        /* Mirror for the right half */
        for (int i = 1; i <= half - 1; ++i)
            digits[len - 1 - i] = digits[i];
    } else {                                   /* even length */
        int free = half - 1;
        int fd[64] = {0};
        idx_t t = k;
        for (int i = free - 1; i >= 0; --i) {
            fd[i] = (int)(t % 8);
            t /= 8;
        }
        int pos = 1;
        for (int i = 0; i < free; ++i)
            digits[pos++] = (uint8_t)fd[i];

        /* Mirror for the right half */
        for (int i = 1; i <= half - 1; ++i)
            digits[len - 1 - i] = digits[i];
    }

    /* Convert the digit array to a value in base 8. */
    seq_t val = 0;
    for (int i = 0; i < (int)len; ++i)
        val = val * (seq_t)8 + (seq_t)digits[i];

    return val;
}
