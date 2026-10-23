/*
 * A178207  a(n) = 4*n - 2*A000120(n) + 1 where A000120(n) = number of nonzero
 *          digits in the binary representation of n.
 * https://oeis.org/A178207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*n - 2*A000120(n) + 1 * where A000120(n) = popcount(n) =
 *            number of 1‑bits in the binary * representation of n
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of set bits in a 64‑bit integer.
   This works on any compiler, no builtin functions needed. */
static unsigned int popcount_u64(uint64_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* Return the nth term of the sequence.  Bounds allow any n ≥ 0. */
seq_t a(idx_t n)
{
    unsigned int pc = popcount_u64((uint64_t)n);
    seq_t res = (seq_t)4 * (seq_t)n;
    res -= (seq_t)2 * (seq_t)pc;
    res += (seq_t)1;
    return res;
}
