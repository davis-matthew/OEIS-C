/*
 * A163403  a(n) = 2*a(n-2) for n > 2; a(1) = 1, a(2) = 2.
 * https://oeis.org/A163403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^floor(n/2)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-2) for n > 2; a(1) = 1, a(2) = 2 *
 * This implementation uses binary exponentiation to avoid
 * overflow from left‑shift on built‑in types and works for
 * arbitrary‑precision seq_t. */
#include <stdint.h>

static seq_t pow2_seq(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp != 0) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    return pow2_seq(n >> 1);
}
