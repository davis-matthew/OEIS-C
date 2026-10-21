/*
 * A051589  Number of 5 X n binary matrices such that any 2 rows have a common
 *          1.
 * https://oeis.org/A051589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32^n - 10*24^n + 30*20^n - 5*18^n + 5*17^n - 70*16^n -
 *            30*15^n + 135*14^n + 30*13^n - 140*12^n - 2*11^n + 130*10^n -
 *            110*9^n + 45*8^n - 10*7^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Local helper to compute term with sign */
    struct term { unsigned base; unsigned coeff; int sign; };
    static const struct term terms[16] = {
        {32, 1,  1},
        {24, 10, -1},
        {20, 30,  1},
        {18, 5,   -1},
        {17, 5,    1},
        {16, 70,  -1},
        {15, 30,  -1},
        {14, 135,  1},
        {13, 30,   1},
        {12, 140, -1},
        {11, 2,   -1},
        {10, 130,  1},
        {9,  110, -1},
        {8,   45,  1},
        {7,  10,   -1},
        {6,   1,   1}
    };

    seq_t result = 0;
    for (int i = 0; i < 16; ++i) {
        seq_t p = pow_uint(terms[i].base, n);
        p *= (seq_t)terms[i].coeff;
        if (terms[i].sign > 0)
            result += p;
        else
            result -= p;
    }
    return result;
}
