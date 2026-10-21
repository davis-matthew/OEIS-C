/*
 * A220982  a(n) = 6^(4n+2) + 6^(3n+2) + 3 * 6^(2n+1) + 6^(n+1) + 1: the right
 *          Aurifeuillian factor of 6^(12n+6) + 1.
 * https://oeis.org/A220982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(4n+2) + 6^(3n+2) + 3 * 6^(2n+1) + 6^(n+1) + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute  base^exp for a general seq_t type. */
static seq_t power_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        result *= base;
        exp--;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t six = (seq_t)6;
    /* compute each power separately */
    seq_t p1 = power_seq(six, 4 * n + 2); /* 6^(4n+2) */
    seq_t p2 = power_seq(six, 3 * n + 2); /* 6^(3n+2) */
    seq_t p3 = power_seq(six, 2 * n + 1); /* 6^(2n+1) */
    seq_t p4 = power_seq(six, n + 1);     /* 6^(n+1)   */

    /* assemble the final value */
    seq_t term3 = (seq_t)3 * p3;
    seq_t result = p1 + p2 + term3 + p4 + (seq_t)1;

    return result;
}
