/*
 * A008938  Degrees of irreducible representations of group U3(3).
 * https://oeis.org/A008938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A008938: Degrees of irreducible representations of group U3(3).
   The group has 14 conjugacy classes, so the sequence is finite:
   1, 6, 7, 7, 7, 14, 21, 21, 21, 27, 28, 28, 32, 32.
   a(n) = 0 for n > 14. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The degrees for n = 1..14.  The sequence ends after 14 terms. */
    static const seq_t degs[14] = {
        1,
        6,
        7,
        7,
        7,
        14,
        21,
        21,
        21,
        27,
        28,
        28,
        32,
        32
    };

    if (n == 0 || n > 14) {
        return 0;
    }
    return degs[n - 1];
}
