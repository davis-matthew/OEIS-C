/*
 * A034981  Smallest square starting with a string of n 2's.
 * https://oeis.org/A034981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest perfect square whose decimal representation
 *            begins with n consecutive '2's
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* construct the prefix consisting of n twos:
       222...2  (n digits) = 2 * (10^n - 1) / 9 */
    seq_t pow10n  = pow10(n);                     /* 10^n */
    seq_t prefix  = (pow10n - 1) / 9 * 2;         /* 222...2 */

    seq_t pow10d = 1;                              /* 10^0 */
    for (;;) {
        seq_t lower = prefix * pow10d;                     /* prefix * 10^d   */
        seq_t upper = (prefix + 1) * pow10d - 1;           /* (prefix+1)*10^d-1 */

        seq_t s_low  = floor_sqrt(lower);                /* floor sqrt(lower) */
        if (s_low * s_low < lower) ++s_low;              /* ceil sqrt(lower) */

        seq_t s_high = floor_sqrt(upper);                /* floor sqrt(upper) */

        if (s_low <= s_high) {
            seq_t res = s_low * s_low;                  /* the square */
            return res;
        }

        /* try next order of magnitude */
        pow10d *= 10;
    }
}
