/*
 * A237633  Number of (n+1)X(4+1) 0..2 arrays with the upper median of every
 *          2X2 subblock differing from its horizontal and vertical neighbors
 *          by exactly one.
 * https://oeis.org/A237633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n‑4) for n>14 Initial values: a(1)=1786, a(2)=2356,
 *            a(3)=1488, a(4)=2012, a(5)=2816, a(6)=4440, a(7)=5976,
 *            a(8)=10104, a(9)=14168, a(10)=22294, a(11)=29908, a(12)=50502,
 *            a(13)=70828, a(14)=111468
 * Range:     u8 none, u16 n <= 12, u32 n <= 40, u64 n <= 95, u128 n <= 205,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static const uint64_t init[15] = {
        0ULL,            /* unused index 0 */
        1786ULL,         /* a(1) */
        2356ULL,         /* a(2) */
        1488ULL,         /* a(3) */
        2012ULL,         /* a(4) */
        2816ULL,         /* a(5) */
        4440ULL,         /* a(6) */
        5976ULL,         /* a(7) */
        10104ULL,        /* a(8) */
        14168ULL,        /* a(9) */
        22294ULL,        /* a(10) */
        29908ULL,        /* a(11) */
        50502ULL,        /* a(12) */
        70828ULL,        /* a(13) */
        111468ULL        /* a(14) */
    };

    if (n <= 14) {
        return (seq_t)init[(size_t)n];
    }

    /* Map n mod 4 to the corresponding base index in the table */
    static const idx_t base_map[4] = {12, 13, 14, 11};
    idx_t rem = n % 4;
    idx_t base = base_map[rem];

    /* How many times the recurrence 5*a(n‑4) is applied */
    idx_t k = (n - base) / 4;

    seq_t result = (seq_t)init[(size_t)base];
    while (k-- > 0) {
        result *= 5;
    }
    return result;
}
