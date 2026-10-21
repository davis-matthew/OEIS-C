/*
 * A051243  Base 3 numbers d(1)...d(j) such that d(i) != d(j+1-i) for all but
 *          2 values of i.
 * https://oeis.org/A051243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A051243: base‑3 numbers having exactly one mirrored equal pair of digits. */

#include <stdint.h>
#include <stddef.h>

/* a(n) returns the n-th term (1‑based) of A051243. */
seq_t a(idx_t n)
{
    /* Pre‑compute powers of 3 up to 3^20 (sufficient for n ≤ 100 000). */
    static idx_t pow3[21];
    static int pow3_inited = 0;
    if (!pow3_inited) {
        pow3[0] = 1;
        for (idx_t i = 1; i < 21; ++i) pow3[i] = pow3[i - 1] * 3;
        pow3_inited = 1;
    }

    /* Find the digit‑length 2k that contains the n‑th term. */
    idx_t cum = 0;         /* terms seen so far */
    idx_t k = 1;           /* half‑length (number of mirrored pairs) */
    idx_t pow6 = 1;        /* 6^(k-2) , starts with 1 for k = 1 */

    idx_t target_k = 1;
    idx_t cum_prev = 0;

    for (;;) {
        idx_t count;              /* number of valid terms for current k */
        if (k == 1) {
            count = 2;            /* 2‑digit numbers: 11₃ and 22₃ */
        } else {
            count = 12 * k * pow6;/* 12 k · 6^(k‑2) */
        }

        if (cum + count >= n) {
            target_k = k;
            cum_prev = cum;
            break;
        }

        cum += count;
        if (k >= 2) pow6 *= 6;    /* prepare pow6 for next k */
        ++k;
    }

    /* Remaining position within the block of length 2·target_k. */
    idx_t offset = n - cum_prev - 1;   /* 0‑based index */

    /* Numerical search within the block of even length. */
    idx_t len   = 2 * target_k;            /* digits count */
    idx_t start = pow3[len - 1];           /* first number with leading digit 1 */
    idx_t end   = pow3[len] - 1;           /* last number with exactly len digits */

    /* Temporary array to hold digits of a candidate number. */
    unsigned char digits[20];

    for (idx_t val = start; val <= end; ++val) {
        /* Decode val into base‑3 digits (most significant first). */
        idx_t tmp = val;
        for (idx_t pos = len; pos > 0; --pos) {
            digits[pos - 1] = tmp % 3;
            tmp /= 3;
        }

        /* Count positions i with d[i] == d[len‑1‑i].  Each mirroring pair is counted twice. */
        idx_t equal = 0;
        for (idx_t pos = 0; pos < len; ++pos) {
            if (digits[pos] == digits[len - 1 - pos]) ++equal;
        }

        if (equal == 2) {        /* exactly one mirrored equal pair */
            if (offset == 0) return (seq_t)val;
            --offset;
        }
    }

    /* Should never reach here for valid (n ≥ 1). */
    return 0;
}
