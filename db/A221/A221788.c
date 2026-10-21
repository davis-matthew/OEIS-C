/*
 * A221788  Number of 3 X n arrays of occupancy after each element moves to
 *          some horizontal or antidiagonal neighbor, without 2-loops or left
 *          turns.
 * https://oeis.org/A221788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 28, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A221788 – Number of 3×n arrays of occupancy after each element moves 
   to some horizontal or antidiagonal neighbor, without 2‑loops or left turns.
   Formula: a(n) = 8*a(n-1) - 14*a(n-2) + 8*a(n-3) - a(n-4)   (n > 6)
   Base values: a(1)=0, a(2)=0, a(3)=0, a(4)=5, a(5)=24, a(6)=142
   The function works for any order of calls and any n ≥ 1.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    static const seq_t base[] = {0, 0, 0, 5, 24, 142};
    if (n <= 6)
        return base[n - 1];

    /* a3 = a(n-3), a4 = a(n-2), a5 = a(n-1), a6 = a(n) */
    seq_t a3 = base[2]; /* a(3) */
    seq_t a4 = base[3]; /* a(4) */
    seq_t a5 = base[4]; /* a(5) */
    seq_t a6 = base[5]; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 8 * a6 - 14 * a5 + 8 * a4 - a3;
        a3 = a4;   /* shift window forward */
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    return a6;
}
