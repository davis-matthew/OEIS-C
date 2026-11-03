/*
 * A074400  Sum of the even divisors of 2n.
 * https://oeis.org/A074400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * σ(n) (sum of the even divisors of 2n)
 * Range:     u8 n <= 59, u16 n <= 9239, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 78 terms through n = 9841 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/isqrt.h"

/* ---------- compute sigma (sum of divisors) for n >= 1 ---------- */
static seq_t _sigma_of(idx_t n)
{
    seq_t sum = 0;
    idx_t root = isqrt(n);
    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t q = n / d;
            if (q != d)
                sum += (seq_t)q;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; index 0 is never used, but we return 0 safely. */
    if (n == 0)
        return (seq_t)0;
    return (seq_t)2 * _sigma_of(n);
}
