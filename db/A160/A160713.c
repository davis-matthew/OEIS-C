/*
 * A160713  3 times numbers of Gould's sequence: a(n) = A001316(n)*3.
 * https://oeis.org/A160713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * 2^{popcount(n)}
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* popcount for 64‑bit unsigned integer */
    uint64_t x = (uint64_t)n;
    int pc = 0;
    while (x) {
        x &= x - 1;
        pc++;
    }

    seq_t res = (seq_t)1;
    res <<= pc;        /* 2^pc */
    res *= (seq_t)3;   /* multiply by 3 */
    return res;
}
