/*
 * A125497  Evil cubes.
 * https://oeis.org/A125497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 805, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Evil cubes: a(n) is the nth cube whose binary representation has an even number of 1's */
#include <stdbool.h>

static inline bool even_parity(seq_t x)
{
    seq_t parity = 0;
    while (x) {
        parity ^= (x & 1);
        x >>= 1;
    }
    return parity == 0;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t i = 0;
    while (1) {
        seq_t cube = (seq_t)i * (seq_t)i * (seq_t)i;
        if (even_parity(cube)) {
            count++;
            if (count == n) return cube;
        }
        i++;
    }
}
