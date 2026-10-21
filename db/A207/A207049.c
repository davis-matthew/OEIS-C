/*
 * A207049  Number of (n+1)X8 0..2 arrays with every 2X3 or 3X2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A207049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * a(n-3) for n > 15 Initial values (for 1 ≤ n ≤ 15):
 *            a(1) = 41589 a(2) = 9582 a(3) = 5850 a(4) = 9144 a(5) = 13752
 *            a(6) = 19200 a(7) = 35904 a(8) = 54528 a(9) = 77376 a(10) =
 *            146304 a(11) = 223200 a(12) = 316032 a(13) = 590592 a(14) =
 *            895104 a(15) = 1264512 The recurrence holds for all n...
 * Range:     u8 none, u16 n <= 8, u32 n <= 32, u64 n <= 80, u128 n <= 176,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial terms for n = 1 .. 15 */
    static const seq_t init[15] = {
        41589ULL, 9582ULL, 5850ULL, 9144ULL, 13752ULL,
        19200ULL, 35904ULL, 54528ULL, 77376ULL, 146304ULL,
        223200ULL, 316032ULL, 590592ULL, 895104ULL, 1264512ULL
    };

    if (n <= 15)
        return init[n - 1];

    /* Find the smallest index m ≡ n (mod 3) that is ≤ 15.
       Each step of the recurrence multiplies the value by 4.
       Since a(k) = 4 * a(k-3), we need floor((n - 15) / 3) multiplications by 4. */
    idx_t m = n;
    idx_t times = 0;
    while (m > 15) {
        m -= 3;
        times++;
    }

    seq_t res = init[m - 1];
    /* Multiply by 4^times by shifting left twice per multiplication.
       Works for all supported seq_t types (uint*_t, unsigned __int128,
       and arbitrary‑precision integers that support operator<<). */
    while (times--)
        res <<= 2;
    return res;
}
