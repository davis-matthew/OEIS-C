/*
 * A171901  Numbers with at least two identical neighboring digits in their
 *          decimal representation.
 * https://oeis.org/A171901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adjacent_repeat_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A171901: the nth positive integer ≥10 that has at least one pair of identical neighboring decimal digits.
   The function performs a linear scan counting such numbers until the nth is found. */
#include <stdbool.h>
#include "../utils/has_adjacent_repeat_uint64.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n<1, but return 0 for safety */

    idx_t found = 0;
    uint64_t x = 11;        /* first number that can satisfy the condition */
    while (found < n)
    {
        if (has_adjacent_repeat_uint64(x))
        {
            found++;
            if (found == n)
                return (seq_t)x;
        }
        x++;
    }
    return (seq_t)x; /* unreachable */
}
