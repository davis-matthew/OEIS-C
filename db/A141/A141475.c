/*
 * A141475  Number of Turing machines with n states following the standard
 *          formalism of the busy beaver problem where the head of a Turing
 *          machine either moves to the right or to the left, but none once
 *          halted.
 * https://oeis.org/A141475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+2)^(2n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */
/* a(1) = 36 */
/* a(2) = 10000 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute base = 4*n + 2, exponent = 2*n */
    seq_t base = (seq_t)(4 * n + 2);
    seq_t result = 1;
    idx_t exp = 2 * n;          /* exponent fits in idx_t for practical n */
    for (idx_t i = 0; i < exp; ++i)
        result *= base;         /* multiply base exp times */
    return result;
}
