/*
 * A130916  a(n) = smallest integer >= n which has only prime factors 2, 3 and
 *          5.
 * https://oeis.org/A130916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer >= n with all prime factors 2, 3, and 5
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 945 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline bool is_hamming(seq_t x)
{
    if (x == 0)
        return false;
    /* remove factors of 2 */
    while ((x & 1) == 0)
        x >>= 1;
    /* remove factors of 3 */
    while (x % 3 == 0)
        x /= 3;
    /* remove factors of 5 */
    while (x % 5 == 0)
        x /= 5;
    return x == 1;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    for (;; ++x)
    {
        if (is_hamming(x))
            return x;
    }
}
