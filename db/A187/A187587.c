/*
 * A187587  Number of 4-step E, S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 58*n^2 - 204*n + 174 for n > 2 a(1) = 0, a(2) = 5
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out‑of‑range, return 0 */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;

    seq_t nn = (seq_t)n;                   /* cast once, use only seq_t arithmetic */
    return (seq_t)(58 * nn * nn - 204 * nn + (seq_t)174);
}
