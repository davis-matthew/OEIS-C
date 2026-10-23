/*
 * A018797  Smallest cube that begins with n.
 * https://oeis.org/A018797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   a(n) = A018852(n)^3 * A018852(n) = the smallest integer m whose
 *            cube begins with the * decimal representation of n. The function
 *            returns m^3. For n = 0 * the definition is a(0) = 0. The
 *            computation is purely * constructive: we try successive m until
 *            the leading digits of * m^3 match n. No caching...
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 430, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/digits.h"     /* int digits(idx_t x);            */
#include "../utils/digit_len.h"  /* int digit_len(seq_t x);         */
#include "../utils/pow10.h"      /* seq_t pow10(idx_t e);           */

seq_t
a(idx_t n)
{
    /* Special case 0: defined in the OEIS entry */
    if (n == 0)
        return (seq_t)0;

    /* Number of decimal digits of n */
    const int d = digits(n);

    for (idx_t m = 1; ; ++m) {
        seq_t sq = (seq_t)m * (seq_t)m * (seq_t)m;         /* cube of m */
        int len = digit_len(sq);                          /* digits of cube */

        if (len < d)
            continue;                                     /* too short */

        /* Extract the leading d digits of the cube */
        seq_t leading = sq / pow10((idx_t)(len - d));

        if (leading == (seq_t)n)  /* we found the smallest m */
            return sq;
    }
}
