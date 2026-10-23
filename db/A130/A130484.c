/*
 * A130484  a(n) = Sum_{k=0..n} (k mod 6) (Partial sums of A010875).
 * https://oeis.org/A130484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*floor(n/6) + (n mod 6)*(n mod 6 + 1)/2
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t q = (seq_t)n / 6;              /* floor(n/6) */
    seq_t r = (seq_t)n % 6;              /* n mod 6 */
    seq_t t = r * (r + 1) / 2;           /* triangular number of r */
    return q * 15 + t;                   /* 15*floor(n/6) + t */
}
