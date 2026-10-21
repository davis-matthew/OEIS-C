/*
 * A067487  Powers of 9 with initial digit 9.
 * https://oeis.org/A067487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 1, u128 n <= 2,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n-th power of 9 whose first decimal
   digit is 9.  The function determines the exponent k
   by inspecting the fractional part of k·log10(9), then
   returns 9^k using the helper seq_pow() which handles
   arbitrary‑precision integers. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

/* Scaling constants for a fixed‑point approximation of log10(9). */
#define LOG10_9_SCALE 10000000000000000ULL          /* 1e16   */
#define LOG10_9_COEFF 9542425094393249ULL           /* round(log10(9)*1e16) */

seq_t a(idx_t n)
{
    /* Counts how many exponents with leading digit 9 have been found. */
    idx_t count = 0;
    /* Current exponent.  Starts at 1 (9^1). */
    idx_t k = 1;

    /* Loop until the n‑th suitable exponent is found. */
    while (true) {
        /* Compute fractional part of k·log10(9) scaled by LOG10_9_SCALE.
           Using 128‑bit arithmetic to avoid overflow. */
        unsigned long long frac =
            (unsigned long long)((__uint128_t)k * LOG10_9_COEFF % LOG10_9_SCALE);

        /* If the fractional part is at least log10(9), the leading digit is 9. */
        if (frac >= LOG10_9_COEFF) {
            ++count;
            if (count == n) {
                /* Return 9^k.  seq_pow handles arbitrary precision if needed. */
                return seq_pow((seq_t)9, k);
            }
        }
        /* Advance to the next exponent. */
        ++k;
    }
}
