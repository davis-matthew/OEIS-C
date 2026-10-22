/*
 * A000172  The Franel number a(n) = Sum_{k = 0..n} binomial(n,k)^3.
 * https://oeis.org/A000172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^3 (fixed-width builds);
 *            (n+1)^2*a(n+1) = (7*n^2+7*n+2)*a(n) + 8*n^2*a(n-1), a(0) = 1,
 *            a(1) = 2 (arbitrary-precision build)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) {
        /* direct sum; binomials stay far smaller than a(n) */
        seq_t c = 1, s = 0;
        idx_t k;
        for (k = 0; k <= n; k++) {
            s += c * c * c;
            if (k < n) {
                c = c * (seq_t)(n - k);
                c = c / (k + 1);
            }
        }
        return s;
    } else {
        seq_t prev, cur, next, m;
        idx_t i;
        if (n == 0)
            return (seq_t)1;
        prev = 1;
        cur = 2;
        for (i = 1; i < n; i++) {
            m = (seq_t)i;
            next = ((7 * m * m + 7 * m + 2) * cur + 8 * m * m * prev)
                   / ((m + 1) * (m + 1));
            prev = cur;
            cur = next;
        }
        return cur;
    }
}
