/*
 * A054683  Numbers whose sum of digits is even.
 * https://oeis.org/A054683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 867 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine computes the OEIS sequence A054683:
 * Numbers whose sum of decimal digits is even.
 * (i.e. integers having an even number of odd digits)
 * a(1) = 0, a(2) = 2, a(3) = 4, ...                          */
/* If you need a faster implementation for very large n,
 * you can generate the terms by the 10‑period recurrence
 * a(n+10) = a(n) + 20 after the first 10 terms.        */
#include <stdbool.h>

/* Check whether the decimal digit sum of an integer x is even.
 * Works for arbitrary sized seq_t because only modulo 10
 * and division by 10 are used. */
static bool even_sum_digits(seq_t x)
{
    seq_t t = x;
    bool parity = false;                  /* true if odd count of odd digits */
    while (t > (seq_t)0) {
        seq_t digit = t % (seq_t)10;
        parity ^= (bool)(digit & (seq_t)1);
        t = t / (seq_t)10;
    }
    return !parity;                       /* true if sum is even */
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    idx_t seen = 0;
    seq_t cur = 0;
    while (true) {
        if (even_sum_digits(cur)) {
            seen++;
            if (seen == n) return cur;
        }
        cur = cur + (seq_t)1;
    }
}
