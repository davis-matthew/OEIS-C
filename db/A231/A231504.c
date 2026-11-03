/*
 * A231504  a(n) = Sum_{i=0..n} digsum_3(i)^3, where digsum_3(i) = A053735(i).
 * https://oeis.org/A231504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=0..n} digsum_3(i)^3 where digsum_3(i) is the sum of
 *            the base‑3 digits of i
 * Range:     u8 n <= 13, u16 n <= 374, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9941 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t in helper */

/* Return the sum of the base‑3 digits of n.  */
static inline unsigned int digsum3(uint64_t n)
{
    unsigned int sum = 0;
    while (n > 0) {
        sum += (unsigned int)(n % 3);
        n /= 3;
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) {
        unsigned int ds = digsum3(i);
        seq_t d   = (seq_t)ds;
        seq_t cub = d * d * d;
        total += cub;
    }
    return total;
}
