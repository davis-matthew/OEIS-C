/*
 * A165669  First digit of the decimal expansion of (n^2+2)/7 after the
 *          decimal point.
 * https://oeis.org/A165669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(n) = floor(10*(n^2+2)/7) mod 10
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert n to the sequence type to avoid overflow when squaring. */
    seq_t nn = (seq_t)n;

    /* Compute r = (n^2 + 2) mod 7. */
    seq_t r = (nn * nn + (seq_t)2) % (seq_t)7;

    /* First digit after the decimal point of (n^2+2)/7. */
    seq_t val = (seq_t)10 * r / (seq_t)7;

    /* val is already in 0..8, but keep the mod 10 to satisfy the OEIS
       definition. */
    return val % (seq_t)10;
}
