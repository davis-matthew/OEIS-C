/*
 * A047573  Numbers that are congruent to {0, 1, 2, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-4)/7) (OEIS A047573)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Return the n-th term of the sequence A047573.
   The function is defined for n >= 1 (offset 1). */
seq_t a(idx_t n)
{
    /* For the first three indices the floor term equals -1. */
    if (n <= 3) {
        /* (seq_t)0,1,2 -> 0,1,2 after subtracting 1. */
        return (seq_t)n - 1;
    }
    /* For n >= 4 the floor term is (n-4)/7. */
    seq_t off = (seq_t)(n - 4) / 7;
    return (seq_t)n + off;
}
