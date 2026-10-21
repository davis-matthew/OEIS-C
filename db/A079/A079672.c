/*
 * A079672  Numbers of the form (3^s+1)/(3^r+1) for s > 1, 1 <= r <= s-1.
 * https://oeis.org/A079672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers of the form (3^s+1)/(3^r+1) with s>1, 1≤r≤s-1 * and s/r
 *            odd. The terms are listed in increasing s, then in * increasing
 *            r that satisfies the conditions
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t count = 0;            /* how many terms have been produced */

    for (idx_t s = 3; ; ++s) {  /* start with the smallest eligible s */
        /* numerator = 3^s + 1 */
        seq_t numerator = pow3(s);
        numerator = numerator + 1;

        for (idx_t r = 1; r < s; ++r) {
            if (s % r != 0) continue;              /* r must divide s */
            if (((s / r) & 1) == 0) continue;        /* ratio s/r must be odd */

            /* denominator = 3^r + 1 */
            seq_t denominator = pow3(r);
            denominator = denominator + 1;

            seq_t val = numerator / denominator;    /* integer division exact */

            ++count;
            if (count == n) return val;
        }
    }
}
