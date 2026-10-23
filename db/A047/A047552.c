/*
 * A047552  Numbers that are congruent to {2, 6, 7} mod 8.
 * https://oeis.org/A047552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3k) = 8k-1; a(3k-1)=8k-2; a(3k-2)=8k-6
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Evaluate the n‑th number congruent to 2, 6 or 7 modulo 8.
       n is guaranteed to be >= 1 (the behaviour for n == 0 is undefined). */
    idx_t q = (n - 1) / 3;        /* block index (0‑based)       */
    idx_t r = (n - 1) % 3;        /* position inside the block   */
    static const seq_t MAP[3] = {2, 6, 7};

    seq_t base = (seq_t)q;        /* 8·base will give the block start */
    return 8 * base + MAP[(size_t)r];
}
