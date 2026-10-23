/*
 * A101243  Slowest increasing sequence where the absolute difference between
 *          the last digit of a(n) and the first digit of a(n+1) equals 9.
 * https://oeis.org/A101243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A101243
   a(0) = 0
   For n ≥ 1: a(n) is the n‑th number that starts with 9, ends with 0,
   and has all middle digits from 0 up to 10^(L‑2)−1, where L is the
   number of digits.  The terms are listed in increasing order. */
#pragma once
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    seq_t m = (seq_t)n - 1;        /* zero‑based index among the 9…0 numbers */

    idx_t L = 2;                   /* current length (≥2) */
    seq_t cumulative;

    /* Find the minimal L such that cumulative > m, where
       cumulative = (10^(L-1) − 1) / 9 = sum_{len=2}^{L} 10^(len-2). */
    for (;;) {
        cumulative = (pow10(L - 1) - 1) / 9;
        if (cumulative > m) break;
        ++L;
    }

    /* Cumulative count up to length L‑1 */
    seq_t cumulative_prev = (pow10(L - 2) - 1) / 9;
    seq_t offset = m - cumulative_prev;          /* 0 ≤ offset < 10^(L-2) */

    seq_t base = 9 * pow10(L - 1);                /* 9 followed by (L‑1) zeros */
    return base + offset * 10;                   /* 9…(offset)0  */
}
