/*
 * A030686  Smallest nontrivial extension of n^2 which is a square.
 * https://oeis.org/A030686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest perfect square that has n² as a non‑trivial
 *            prefix in base 10 (i.e., at least one digit appended)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow10, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sqrt_floor.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* base = n² */
    seq_t base = (seq_t)n * (seq_t)n;

    /* pow10k = 10^k, start with k = 1 */
    seq_t pow10k = pow10((idx_t)1);

    for (idx_t k = 1;; ++k)
    {
        /* interval [low, high] that contains all numbers starting with base
           followed by k decimal digits. */
        seq_t low  = base * pow10k;
        seq_t high = (base + (seq_t)1) * pow10k - (seq_t)1;

        /* smallest integer whose square is >= low */
        seq_t root = sqrt_floor(low);
        if (root * root < low)
            ++root;

        /* check if this root² lies in the interval */
        seq_t square = root * root;
        if (square <= high)
            return square;

        /* prepare next power of 10 for the next k */
        pow10k *= (seq_t)10;
    }
}
