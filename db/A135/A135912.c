/*
 * A135912  Number of 5-tuples (x,y,z,t,u) of nonnegative integers such that
 *          x^2+y^3+z^4+t^5+u^6 = n.
 * https://oeis.org/A135912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5‑tuples (x,y,z,t,u) of non‑negative integers
 *            such that x^2 + y^3 + z^4 + t^5 + u^6 = n
 * Range:     u8 n <= 88, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute base^exp but stop and return >limit to avoid overflow or exceeding limit. */
static seq_t power_limit(idx_t base, int exp, seq_t limit)
{
    if (base == 0) return 0;
    seq_t v = 1;
    for (int i = 0; i < exp; ++i) {
        if (v > limit / base) return limit + 1;   /* would overflow or exceed limit */
        v *= base;
    }
    return v;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t x = 0;; ++x) {
        seq_t sx = power_limit(x, 2, n);
        if (sx > n) break;

        for (idx_t y = 0;; ++y) {
            seq_t sy = power_limit(y, 3, n);
            seq_t sum1 = sx + sy;
            if (sum1 > n) break;

            for (idx_t z = 0;; ++z) {
                seq_t sz = power_limit(z, 4, n);
                seq_t sum2 = sum1 + sz;
                if (sum2 > n) break;

                for (idx_t t = 0;; ++t) {
                    seq_t st = power_limit(t, 5, n);
                    seq_t sum3 = sum2 + st;
                    if (sum3 > n) break;

                    /* Iterate over possible u^6 values. */
                    seq_t remaining = n - sum3;
                    for (idx_t u = 0;; ++u) {
                        seq_t su = power_limit(u, 6, n);
                        if (su > remaining) break;
                        if (su == remaining) ++count;
                    }
                }
            }
        }
    }
    return count;
}
