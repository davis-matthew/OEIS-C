/*
 * A068071  Triangular numbers that remain triangular when the least
 *          significant digit is moved to the beginning.
 * https://oeis.org/A068071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   digit_len, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A068071: Triangular numbers that remain triangular when their
 * least significant decimal digit is moved to the front.
 *
 * The implementation generates triangular numbers T(k)=k(k+1)/2 and
 * checks whether the decimal rotation of T(k) is also triangular.
 * A number x is triangular iff 8*x+1 is a perfect square.
 *
 * Helper functions from "../utils" are used:
 *   - floor_sqrt(seq_t)   : integer square root
 *   - digit_len(seq_t)    : number of decimal digits
 *   - pow10(idx_t)        : 10^e
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

static bool is_triangular(seq_t x)
{
    seq_t v = (seq_t)8 * x + 1;
    seq_t s = floor_sqrt(v);
    return s * s == v;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t found = 0;          // count of qualifying triangular numbers
    for (idx_t k = 1; ; ++k)  // iterate over triangular numbers
    {
        seq_t tri = (seq_t)k * ((seq_t)k + 1) / 2;

        // rotate: move least significant decimal digit to the front
        seq_t d   = tri % 10;                      // last digit
        seq_t rest = tri / 10;                      // remaining digits
        idx_t  m_digits = digit_len(tri);           // total decimal digits
        seq_t rotated = d * pow10(m_digits - 1) + rest;

        if (is_triangular(rotated))
        {
            ++found;
            if (found == n)
                return tri;
        }
    }
}
