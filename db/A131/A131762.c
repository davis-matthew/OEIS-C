/*
 * A131762  Number of 1s in the 1's complement of the 32-bit binary
 *          representation of n.
 * https://oeis.org/A131762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, fini
 * Formula:   a(n) = 32 - (number of 1s in 32‑bit binary representation of n)
 * Range:     u8 n <= 4095, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   popcount_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/popcount_u32.h"

seq_t a(idx_t n)
{
    /* Only the lower 32 bits matter for the 1's complement of a 32‑bit value. */
    uint32_t v = (uint32_t)n;
    unsigned int pc = popcount_u32(v);
    return (seq_t)32 - (seq_t)pc;
}
