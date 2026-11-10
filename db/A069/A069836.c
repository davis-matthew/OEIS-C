/*
 * A069836  Inverse permutation to A057033: a(n) is the m such that A057033(m)
 *          = n, or 0 if no such m exists.
 * https://oeis.org/A069836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = m such that A057033(m) = n; computed by tracking the
 *            element n through the permutations P(2), P(3), ...: at step k,
 *            if p >= k then p -> p+k-1 when k divides p, else p -> p-1; stop
 *            when k > p. The final p is a(n)
 * Range:     u8 n <= 19, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p = (seq_t)n;
    seq_t k = 2;
    while (k <= p) {
        if (p % k == 0)
            p += k - 1;
        else
            p -= 1;
        ++k;
    }
    return p;
}
