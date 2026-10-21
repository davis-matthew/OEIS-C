/*
 * A061858  Differences between the ordinary multiplication table A004247 and
 *          the carryless multiplication table for GF(2)[X] polynomials
 *          A048720, i.e., the effect of the carry bits in binary
 *          multiplication.
 * https://oeis.org/A061858
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = A004247(n) - A048720(n) where A004247(n) is the ordinary
 *            multiplication table enumerated by anti‑diagonals (row + column
 *            = d), and A048720(n) is the carryless (polynomial) product of
 *            the same pair. The sequence is zero for the first 24 indices
 *            because each pair up to (3,3) yields no carry
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   _floor_isqrt_u64, carryless_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/carryless_mul.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = ( _floor_isqrt_u64(8 * n + 1) - 1 ) / 2;

    /* Position within that diagonal */
    idx_t prev = d * (d + 1) / 2;    /* elements before diagonal d */
    idx_t p   = n - prev;            /* 0‑based offset inside diagonal */

    /* Map to column and row indices (i, j) */
    idx_t i = p;
    idx_t j = d - p;

    /* Ordinary product */
    seq_t ordinary = (seq_t)i * (seq_t)j;

    /* Carryless (polynomial) product over GF(2) */
    seq_t carryless = carryless_mul((seq_t)i, (seq_t)j);

    return ordinary - carryless;
}
