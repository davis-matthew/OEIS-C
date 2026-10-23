/*
 * A028836  Iterated sum of digits of n is a power of 2.
 * https://oeis.org/A028836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9*⌊(n-1)/4⌋ + {1,2,4,8}[(n-1) mod 4]
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t offsets[4] = {1, 2, 4, 8};

    /* Number of complete 9‑number blocks preceding the desired term. */
    idx_t k = (n - 1) / 4;

    /* Position within the current block (0‑based). */
    idx_t r = (n - 1) % 4;

    return (seq_t)k * 9 + offsets[(size_t)r];
}
