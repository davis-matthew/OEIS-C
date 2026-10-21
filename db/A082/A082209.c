/*
 * A082209  a(1) = 1, a(n) = smallest number such that the concatenation of
 *          a(n-1) and a(n) is a square.
 * https://oeis.org/A082209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Pattern: a(n) = [1, 6, 4, 9, 61, 504] repeated every 6 steps. */

#include <stddef.h>

static const seq_t period[6] = { 1, 6, 4, 9, 61, 504 };

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out of range – never used by tests */
    return period[(n - 1) % 6];
}
