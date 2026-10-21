/*
 * A226583  Smallest number of integer-sided squares needed to tile a 10 X n
 *          rectangle.
 * https://oeis.org/A226583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-10)+1 for n>12; a(0..12) =
 *            {0,10,5,6,4,2,4,6,5,6,1,6,5}
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    static const seq_t base[13] = {0,10,5,6,4,2,4,6,5,6,1,6,5};
    if (n <= 12) return base[(size_t)n];
    idx_t k = (n - 3) / 10;          /* number of times 10 is subtracted */
    idx_t m = n - 10 * k;            /* remaining index (0..12) */
    return base[(size_t)m] + (seq_t)k;
}
