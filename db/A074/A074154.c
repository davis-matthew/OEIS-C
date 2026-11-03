/*
 * A074154  Largest n-digit power of n; or 0 if no such number exists.
 * https://oeis.org/A074154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow_u64_idx, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the largest n‑digit power of n; returns 0 if no
   n‑digit power of n exists. */
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/num_digits.h"

seq_t a(idx_t n)
{
    /* Edge case n = 1: 1^k = 1 has one digit. */
    if (n == 1u)
        return (seq_t)1;

    /* For n > 1, log10(n) is positive. */
    double log10_n = log10((double)n);
    if (log10_n <= 0.0)
        return (seq_t)0;

    /* Upper bound for the exponent: k < n / log10(n). */
    idx_t kmax = (idx_t)floor((double)n / log10_n);
    if (kmax == 0u)
        return (seq_t)0;

    /* Search downward for the largest k with n^k having exactly n digits. */
    for (idx_t exp = kmax; exp; --exp)
    {
        seq_t val = ipow_u64_idx((uint64_t)n, exp);
        if (num_digits(val) == n)
            return val;
    }
    return (seq_t)0;
}
