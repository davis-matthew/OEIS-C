/*
 * A102419  "Dropping time" in 3x+1 problem starting at n (number of steps to
 *          reach a lower number than starting value); a(1) = 0 by convention.
 *          Also called glide(n).
 * https://oeis.org/A102419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Collatz steps until the sequence first drops
 *            below starting value n; a(1) = 0 by convention
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;                 /* a(1) is defined to be 0 */

    /* Use an unsigned 128-bit type for safe intermediate values during the Collatz step;
       this works both in C (GNU/Clang) and in C++17 where __uint128_t is available. */
    unsigned __int128 x = n;
    idx_t steps = 0;

    while (x >= n) {
        if (x & 1)          /* odd */
            x = x * 3 + 1;
        else                /* even */
            x >>= 1;
        steps++;
    }

    return (seq_t)steps;
}
