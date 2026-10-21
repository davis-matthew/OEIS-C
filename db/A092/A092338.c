/*
 * A092338  a(n) = number of numbers d with n mod d <= 1.
 * https://oeis.org/A092338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000005(n) + (n>1)*(A000005(n-1) - 1) (where A000005 is
 *            the divisor‑count function τ(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* helper: divisor count τ(x) for x ≥ 1 */
static seq_t divisor_count(idx_t x)
{
    /* n is small enough that a uint64_t loop with d*d <= x is safe */
    uint64_t count = 0;
    for (idx_t d = 1; d * d <= x; ++d) {
        if (x % d == 0) {
            ++count;
            if (d != x / d) {
                ++count;
            }
        }
    }
    return (seq_t)count;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined for offset 1, but safe for callers */
    }
    if (n == 1) {
        return (seq_t)1;   /* a(1) = 1 */
    }
    seq_t tau_n  = divisor_count(n);
    seq_t tau_n1 = divisor_count(n - 1);
    return tau_n + tau_n1 - (seq_t)1;
}
