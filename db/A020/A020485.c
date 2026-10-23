/*
 * A020485  Least positive palindromic multiple of n, or 0 if none exists.
 * https://oeis.org/A020485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive palindrome divisible by n, or 0 if none
 *            exists. (equivalently a(n) = n * A050782(n))
 * Range:     u8 n <= 12, u16 n <= 80, u32 n <= 161, u64 n <= 8990,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow10.h"   /* pow10(idx_t) returns seq_t */

/* Reverse the decimal digits of an arbitrary-precision number. */
static seq_t reverse_seq(seq_t x)
{
    seq_t r = 0;
    while (x > 0) {
        r = r * 10 + (x % 10);
        x /= 10;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* a(0) = 0, and any multiple of 10 has no palindromic multiple. */
    if (n == 0 || (n % 10) == 0)
        return 0;

    seq_t mod_n = (seq_t)n;

    for (idx_t len = 1; ; ++len) {
        /* half = ceil(len/2) ---- number of digits to generate once. */
        idx_t half = (len + 1) / 2;

        /* Compute start = 10^(half-1) and end = 10^half - 1 as idx_t. */
        idx_t start = 1;
        for (idx_t i = 1; i < half; ++i)
            start *= 10;
        idx_t end = start * 10 - 1;

        /* Pre‑compute powers of 10 needed for palindrome construction. */
        seq_t pow10_half = pow10(half);                /* 10^half   */
        seq_t pow10_half_minus1 = pow10(half - 1);    /* 10^(half-1) */

        for (idx_t left = start; left <= end; ++left) {
            seq_t left_seq = (seq_t)left;
            seq_t pal;

            if (len & 1) { /* odd length */
                seq_t rev = reverse_seq(left_seq / 10);
                pal = left_seq * pow10_half_minus1 + rev;
            } else { /* even length */
                seq_t rev = reverse_seq(left_seq);
                pal = left_seq * pow10_half + rev;
            }

            if (pal % mod_n == 0)
                return pal;
        }
    }
}
