/*
 * A231669  a(n) = Sum_{i=0..n} digsum_5(i)^2, where digsum_5(i) = A053824(i).
 * https://oeis.org/A231669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} (digsum_5(i))^2, where digsum_5(i) = sum of
 *            the base‑5 digits of i (A053824)
 * Range:     u8 n <= 17, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the digit sum of x in base 5. */
static inline seq_t digsum5(idx_t x)
{
    seq_t s = 0;
    while (x > 0) {
        s += x % 5;
        x /= 5;
    }
    return s;
}

/* a(n) = Σ_{i=0..n} (digsum_5(i))^2 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t d = digsum5(i);
        sum += d * d;
    }
    return sum;
}
