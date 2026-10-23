/*
 * A153733  Remove all trailing 1's in the binary representation of n.
 * https://oeis.org/A153733
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is even else a((n-1)/2)
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) removes all trailing 1's from the binary representation of n. */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;          /* work on seq_t values only */
    /* repeatedly strip a trailing 1 by dividing by 2 after subtracting 1 */
    while ((res & 1) != 0) {
        /* res-1 is even, shifting right by 1 divides by 2 */
        res = (res - 1) >> 1;
    }
    return res;
}
