/*
 * A125912  Sprague-Grundy values for octal game .141.
 * https://oeis.org/A125912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a is eventually periodic with period 11 starting at n = 14. The
 *            first 24 terms are given explicitly
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* first 24 terms of the sequence (index starts at 1) */
    static const seq_t seq_init[24] = {
        1, 0, 1, 1, 0, 1, 2, 2, 1, 2,
        4, 1, 0, 1, 1, 2, 1, 2, 2, 1,
        2, 4, 1, 2
    };

    /* use the precomputed table for the first 24 indices */
    if (n <= 24) {
        return seq_init[(size_t)(n - 1)];
    }

    /* for n ≥ 14 the sequence repeats every 11 terms:
       a(n) = a( (n‑14) % 11 + 14 )  */
    size_t offset = (size_t)((n - 14) % 11);
    return seq_init[13 + offset];
}
