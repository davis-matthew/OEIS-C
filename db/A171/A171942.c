/*
 * A171942  Forward van Eck transform of A000120.
 * https://oeis.org/A171942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057168(n‑1) – (n‑1) (A057168(n) is the smallest integer
 *            > n with the same number of 1‑bits as n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n = 1 (offset) the difference is 0. */
    if (n <= 1)
        return 0;

    seq_t x = (seq_t)(n - 1);        /* current index (0‑based) */

    /* Gosper's hack: next higher integer with the same popcount. */
    seq_t c = x & -(seq_t)x;         /* lowest set bit */
    seq_t r = x + c;
    seq_t next = (((r ^ x) >> 2) / c) | r;

    return next - x;                 /* difference to the next equal‑weight number */
}
