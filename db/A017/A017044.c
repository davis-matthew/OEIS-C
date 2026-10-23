/*
 * A017044  a(n) = (7*n + 5)^4.
 * https://oeis.org/A017044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 35, u64 n <= 9361, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t etc. */

seq_t a(idx_t n)
{
    /* convert index to seq_t early */
    seq_t n_seq = (seq_t)n;
    /* compute 7*n + 5 */
    seq_t x = 7 * n_seq + 5;
    /* compute (7*n + 5)^2 using 128‑bit intermediate to avoid overflow */
    seq_t x2 = (seq_t)((__int128)x * x);
    /* compute the fourth power: ((7*n + 5)^2)^2 */
    seq_t result = (seq_t)((__int128)x2 * x2);
    return result;
}
