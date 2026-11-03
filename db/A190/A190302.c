/*
 * A190302  Smallest number h such that the decimal expansion of n*h starts
 *          with 1.
 * https://oeis.org/A190302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 777, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9340 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  OEIS A190302 – Smallest h such that the decimal expansion of n*h starts with 1.
 *  a(n) = A187285(n) / n, where A187285(n) is the smallest multiple of n whose decimal
 *  representation begins with the digit 1.  This implementation performs a
 *  straightforward search over successive multipliers h until the leading digit
 *  of n*h equals 1.
 *
 *  The algorithm is O(h·log(n*h)) with h ≤ 5 in all known cases (conjectured to
 *  always hold).  For arbitrary values of n it keeps searching until it finds the
 *  required multiple; the result always fits in seq_t if the corresponding
 *  multiple does.
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

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
    /* start with multiplier 1 and increment until the product starts with 1 */
    idx_t h = 1;
    for (;; ++h) {
        seq_t prod = (seq_t)n * (seq_t)h;        /* n * h */
        int     d   = num_digits(prod);           /* number of decimal digits */
        seq_t   pw  = pow10((idx_t)d - 1);        /* 10^(d‑1)  */
        seq_t   lead= prod / pw;                 /* leading digit */
        if (lead == 1)
            return (seq_t)h;                     /* a(n) = h */
    }
}
