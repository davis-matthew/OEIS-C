/*
 * A006302  Related to enumeration of rooted maps.
 * https://oeis.org/A006302
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*a(n-1) + 4*(binomial(2n-2, n-2) - binomial(2n-2, n-3)),
 *            a(1) = 0 (the added term is the ballot number
 *            3*(2m)!/((m+2)!*(m-1)!) with m = n-1: 1, 3, 9, 28, 90, ...; this
 *            formula was fitted to the listed terms)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   a006302_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006302_gcd.h"

seq_t a(idx_t n)
{
    seq_t s = 0;
    seq_t b = 1; /* ballot number for m = 1 */
    idx_t m;
    for (m = 1; m + 1 <= n; m++) {
        s = s * 4 + b * 4;
        if (m + 1 < n) {
            /* b(m+1) = b(m) * (2m+2)(2m+1) / ((m+3) m), exact */
            uint64_t p = (uint64_t)(2 * m + 2) * (uint64_t)(2 * m + 1);
            uint64_t q = (uint64_t)(m + 3) * (uint64_t)m;
            uint64_t r = (uint64_t)(long long)(b % (seq_t)q);
            uint64_t g = a006302_gcd(r, q);
            b = b / (seq_t)g;
            q /= g;
            b = b * (seq_t)(p / q);
        }
    }
    return s;
}
