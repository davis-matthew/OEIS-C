/*
 * A161764  a(n) is the largest multiple of {the number of 1's in the binary
 *          representation of n} that is <= n.
 * https://oeis.org/A161764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(n / popcount(n)) * popcount(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute the number of 1 bits in a 64‑bit unsigned integer */
static unsigned popcount_u64(uint64_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the problem statement */
    unsigned pc = popcount_u64(n);        /* popcount is at least 1 */
    seq_t t = (seq_t)(n / pc);           /* integer division */
    return t * pc;                       /* largest multiple of pc <= n */
}
