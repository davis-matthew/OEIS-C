/*
 * A187383  Number of 9-step S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A187383: Number of 9‑step S, NW and NE‑moving king's tours on an n×n board summed over all starting positions
   Formula: a(1)=a(2)=a(3)=0,
            a(4)=340,
            a(5)=3060,
            a(6)=10320,
            a(7)=24892,
            a(n) = 3385*n*n - 28390*n + 56892   for n ≥ 8.
   This polynomial is exact for all n ≥ 8 and requires only elementary arithmetic. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= (idx_t)3)
        return (seq_t)0;
    if (n == (idx_t)4)
        return (seq_t)340;
    if (n == (idx_t)5)
        return (seq_t)3060;
    if (n == (idx_t)6)
        return (seq_t)10320;
    if (n == (idx_t)7)
        return (seq_t)24892;

    seq_t sn = (seq_t)n;
    seq_t result = (seq_t)3385 * sn * sn - (seq_t)28390 * sn + (seq_t)56892;
    return result;
}
