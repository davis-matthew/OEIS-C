/*
 * A111897  Number of numbers m <= n such that 7 equals the second digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + [second decimal digit of sqrt(n) == 7], a(0)=0 */
/* Counts how many m ≤ n have 7 as the second digit after the decimal point in sqrt(m). */

#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t m = 1; m <= n; ++m) {
        /* Compute floor(sqrt(m) * 100) without floating‑point:
           floor(sqrt(m) * 100) = floor(sqrt(m * 10000)) */
        idx_t prod = (idx_t)((unsigned __int128)m * 10000u);
        idx_t s    = int_sqrt(prod);

        /* second decimal digit = last digit of s */
        seq_t d = (seq_t)(s % 10);
        if (d == 7)
            ++count;
    }
    return count;
}
