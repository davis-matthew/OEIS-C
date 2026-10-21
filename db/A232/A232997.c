/*
 * A232997  Number of tilings of a fortress (or Penta-Aztec-Diamond) of order
 *          n.
 * https://oeis.org/A232997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^⌊n²/4⌋ if n mod 4 ≠ 3, * a(n) = 2·5^⌊n²/4⌋ if n mod 4 =
 *            3 * * Uses the provided big_pow function which accepts a base
 *            and an exponent * of type seq_t and returns the power as a seq_t
 *            value
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for standard integer literals */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exp = floor(n^2 / 4) using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 prod = (unsigned __int128)n * n;
    seq_t exp = (seq_t)(prod / 4);

    /* Compute 5^exp. */
    seq_t result = big_pow((seq_t)5, exp);

    /* If n ≡ 3 (mod 4), multiply by 2. */
    if ((n & 3) == 3) {
        result *= (seq_t)2;
    }

    return result;
}
