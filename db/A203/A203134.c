/*
 * A203134  Decagonal hexagonal numbers.
 * https://oeis.org/A203134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1154*a(n-1) - a(n-2) + 396, with a(1) = 1, a(2) = 1540
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1540;

    /* Iteratively compute the sequence using the 2‑term recurrence */
    seq_t a1 = (seq_t)1;           /* a(1) */
    seq_t a2 = (seq_t)1540;        /* a(2) */
    seq_t ai;                      /* current term */

    for (idx_t i = 3; i <= n; ++i) {
        ai = (seq_t)1154 * a2 - a1 + (seq_t)396;
        a1 = a2;
        a2 = ai;
    }

    return a2;
}
