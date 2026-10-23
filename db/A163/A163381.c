/*
 * A163381  a(n) = the (decimal equivalent of the) smallest integer that can
 *          be made by rotating the binary digits of n any number of positions
 *          to the left or right. a(n) in binary may contain fewer digits,
 *          after leading 0's are removed, than n written in binary has.
 * https://oeis.org/A163381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = min{ ((n << k) & ((1<<L)-1)) | (n >> (L-k)) | k=0..L-1 },
 *            where L = floor(log2(n)) + 1 is the number of binary digits of n
 * Range:     u8 n <= 386, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* compute bit length L */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }

    seq_t min = (seq_t)n;
    if (bits == 0) return min;

    /* build a mask with bits set to 1 in the low 'bits' positions */
    seq_t mask;
    if (bits == 64 && sizeof(seq_t) == 8) {
        mask = ~((seq_t)0);           /* all 64 bits set */
    } else {
        mask = ((seq_t)1 << bits) - 1;
    }

    seq_t val = (seq_t)n;
    for (idx_t k = 1; k < bits; ++k) {
        seq_t rot = ((val << k) & mask) | (val >> (bits - k));
        if (rot < min) min = rot;
    }
    return min;
}
