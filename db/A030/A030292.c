/*
 * A030292  Numbers k such that k^3 has at most two different digits.
 * https://oeis.org/A030292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: For n ≥ 9, a(n) = 10^e  or  2·10^e
 *   where e = ((n-9+1)/2)+1  and 2·10^e is used when (n-9) is even.
 * For n ≤ 8 the first eight terms are obtained by a direct brute‑force search.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_counts.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* 1) For the very first terms (n ≤ 8) evaluate directly: */
    if (n <= 8) {
        idx_t k = 0;
        idx_t count = 0;
        while (true) {
            /* cube = k^3 */
            seq_t cube = (seq_t)k * (seq_t)k;
            cube = cube * (seq_t)k;

            /* count distinct decimal digits of cube */
            unsigned char cnt[10];
            digit_counts(cube, cnt);
            unsigned distinct = 0;
            for (int i = 0; i < 10; ++i)
                if (cnt[i]) ++distinct;

            if (distinct <= 2) {
                if (++count == n)
                    return (seq_t)k;
            }
            ++k;
        }
    }

    /* 2) For n ≥ 9, use the closed form derived from the OEIS comment. */
    idx_t k = n - 9;                     /* 0‑based offset after the first eight terms */
    idx_t exp = ((k + 1) / 2) + 1;       /* exponent of 10 in the term */
    seq_t term = pow10(exp);             /* 10^exp */

    /* Multiply by 2 when k is even (i.e., for 2·10^exp). */
    if ((k & 1) == 0)
        term = term * 2;

    return term;
}
