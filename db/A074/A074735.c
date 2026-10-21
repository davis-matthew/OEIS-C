/*
 * A074735  Number of steps to reach an integer starting with (n+3)/4 and
 *          iterating the map x -> x*ceiling(x).
 * https://oeis.org/A074735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   _gcd_seqt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074735 : Number of steps to reach an integer starting from (n+3)/4
   and repeatedly replacing x by x*ceil(x).  The algorithm uses a
   Euclidean gcd for rational reduction at each step, with all
   arithmetic carried out in seq_t.
*/
#include <stdbool.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t , seq_t) */
#include "../utils/_gcd_seqt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined, but safe fallback */

    seq_t num = (seq_t)n + 3;       /* numerator of (n+3)/4               */
    seq_t den = 4;                  /* denominator                         */

    /* Reduce initial fraction */
    seq_t g = _gcd_seqt(num, den);
    num /= g;
    den /= g;

    seq_t steps = 0;

    /* Iterate until the rational becomes an integer (den divides num) */
    while (num % den != 0) {
        /* ceil(num/den) = (num + den - 1) / den   */
        seq_t ceil_val = (num + den - 1) / den;

        /* Multiply and reduce again */
        seq_t new_num = num * ceil_val;
        seq_t new_den = den;

        seq_t g2 = _gcd_seqt(new_num, new_den);
        new_num /= g2;
        new_den /= g2;

        num = new_num;
        den = new_den;
        ++steps;
    }

    return steps;
}
