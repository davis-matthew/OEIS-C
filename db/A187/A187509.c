/*
 * A187509  Number of 4-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1 2 for n = 2 21*n^2 - 68*n + 51 for n > 2
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 2;

    /* compute using 64‑bit intermediate to avoid overflow in tiny types */
    unsigned long long tn = n;
    unsigned long long res = 21ULL * tn * tn - 68ULL * tn + 51ULL;
    return (seq_t)res;
}
