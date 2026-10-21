/*
 * A060632  a(n) = 2^wt(floor(n/2)) (i.e., 2^A000120(floor(n/2)), or
 *          A001316(floor(n/2))).
 * https://oeis.org/A060632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^popcount(floor(n/2))
 * Range:     u8 n <= 509, u16 n <= 9550, u32 n <= 9550, u64 n <= 9550,
 *            u128 n <= 9550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* That is, a(n) = 2 raised to the number of 1‑bits in the binary expansion of n/2. */

seq_t a(idx_t n)
{
    uint64_t half = n >> 1;          /* floor(n/2), safe because idx_t is 64-bit unsigned */
    unsigned int pc = 0;             /* popcount of half */
    uint64_t tmp = half;
    while (tmp) {                    /* Kernighan's algorithm */
        tmp &= tmp - 1;
        pc++;
    }

    seq_t result = (seq_t)1;         /* start with 2^0 */
    result <<= pc;                   /* shift left by pc to get 2^pc */
    return result;
}
