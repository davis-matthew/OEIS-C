/*
 * A033478  3x+1 sequence beginning at 3.
 * https://oeis.org/A033478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=3,a(1)=10,a(2)=5,a(3)=16,a(4)=8,a(5)=4,a(6)=2,a(7)=1;
 *            a(n)=a(n-3) for n>7 (collapsing the 4‑2‑1 cycle)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[8] = {3, 10, 5, 16, 8, 4, 2, 1};
    if (n < 8)
        return base[(size_t)n];

    static const seq_t cycle[3] = {2, 1, 4};   /* 4‑2‑1 repeating with period 3 */
    return cycle[(size_t)(n % 3)];
}
