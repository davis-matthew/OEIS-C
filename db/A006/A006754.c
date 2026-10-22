/*
 * A006754  The generalized Conway-Guy sequence w^{0}.
 * https://oeis.org/A006754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n+1) = 2*a(n) - a(n-r(n)) for n >= 4, with r(n) =
 *            round(sqrt(2n)) (Conway-Guy rule), except r(4)=r(5)=r(6)=4 and
 *            r(8)=5, as the data shows; a(0)=0, a(1)=3, a(2)=5, a(3)=6,
 *            a(4)=7
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   a000037_isqrt, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/grow_zero.h"

static idx_t rwin(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n;
    uint64_t r = a000037_isqrt(m);
    if (m > r * r + r) r++;
    if (n == 4 || n == 5 || n == 6) r = 4;
    if (n == 8) r = 5;
    return (idx_t)r;
}

seq_t a(idx_t n)
{
    static seq_t *t = NULL;
    static idx_t len = 0;
    if (n < len) return t[n];
    if (len == 0) {
        t = grow_zero(NULL, 0, 5);
        t[0] = 0; t[1] = 3; t[2] = 5; t[3] = 6; t[4] = 7;
        len = 5;
    }
    if (n >= len) {
        idx_t newlen = n + 1;
        if (newlen < 2 * len) newlen = 2 * len;
        t = grow_zero(t, len, newlen);
        for (idx_t k = len; k < newlen; k++) {
            idx_t m = k - 1;
            t[k] = t[m] + t[m] - t[m - rwin(m)];
        }
        len = newlen;
    }
    return t[n];
}
