/*
 * A065958  a(n) = n^2*Product_{distinct primes p dividing n} (1+1/p^2).
 * https://oeis.org/A065958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^2 * Π_{p|n} (1 + 1/p^2) Equivalent multiplicative form:
 *            a(p^e) = p^(2e) + p^(2e-2)
 * Range:     u8 n <= 14, u16 n <= 221, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 9868 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    idx_t t = n;
    seq_t res = 1;

    /* handle factor 2 */
    idx_t e = 0;
    while ((t & 1) == 0) {
        t >>= 1;
        e++;
    }
    if (e > 0) {
        seq_t term = ipow_u64_idx(2, 2 * e) + ipow_u64_idx(2, 2 * e - 2);
        res *= term;
    }

    /* trial division with odd numbers */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            e = 0;
            while (t % p == 0) {
                t /= p;
                e++;
            }
            seq_t term = ipow_u64_idx(p, 2 * e) + ipow_u64_idx(p, 2 * e - 2);
            res *= term;
        }
    }

    /* remaining prime factor, if any */
    if (t > 1) {
        /* exponent is 1 */
        seq_t term = ipow_u64_idx(t, 2) + 1;
        res *= term;
    }

    return res;
}
