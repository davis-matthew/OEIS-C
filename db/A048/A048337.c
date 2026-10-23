/*
 * A048337  a(n) in base 13 is a repdigit.
 * https://oeis.org/A048337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n - 12*⌊(n-1)/12⌋)*(13^⌊(n+11)/12⌋ - 1)/12
 * Range:     u8 n <= 25, u16 n <= 50, u32 n <= 100, u64 n <= 205,
 *            u128 n <= 412, big any n
 * Helpers:   repdigit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/repdigit_sum.h"

seq_t a(idx_t n)
{
    /* Handle the offset.  The definition in the OEIS is a(0)=0. */
    if (n == 0) return (seq_t)0;

    /* Compute floor((n-1)/12) and floor((n+11)/12) for unsigned n. */
    idx_t lower = (n - 1) / 12;
    idx_t k     = (n + 11) / 12;

    /* Digit d = n - 12*floor((n-1)/12), 1 <= d <= 12 for n>0. */
    idx_t d = n - 12 * lower;

    /* The repdigit multiplier: (13^k - 1) / 12 = 1 + 13 + … + 13^(k-1). */
    seq_t rep = repdigit_sum(k);

    /* Final result.  Multiplication by d (≤12) cannot overflow when
     * a(n) fits in seq_t. */
    return ((seq_t)d) * rep;
}
