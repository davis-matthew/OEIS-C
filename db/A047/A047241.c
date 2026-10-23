/*
 * A047241  Numbers that are congruent to {1, 3} mod 6.
 * https://oeis.org/A047241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*floor((n-1)/2) + (n%2 ? 1 : 3)
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent formulations: a(n) = 2*floor((n-1)/2) + 2*n - 1 */
/* The sequence consists of all positive integers congruent to 1 or 3 (mod 6). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be at least 1 (OEIS offset 1). */
    seq_t k = (seq_t)((n - 1) / 2);   /* floor((n-1)/2) */
    seq_t result = 6 * k;             /* 6*floor((n-1)/2) */
    /* Add offset depending on parity of n */
    result += (n & 1) ? 1 : 3;
    return result;
}
