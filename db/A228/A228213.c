/*
 * A228213  Number of second differences of arrays of length n + 2 of numbers
 *          in 0..3.
 * https://oeis.org/A228213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^(2+n) - 3^(2+n) for n>5
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For the first 5 terms the values are given directly. */

#include "../utils/pow4.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 1: return 13;
        case 2: return 103;
        case 3: return 625;
        case 4: return 3151;
        case 5: return 14053;
        /* For n>=6 use the closed form */
        default: return pow4(n + 2) - pow3(n + 2);
    }
}
