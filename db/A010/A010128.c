/*
 * A010128  Continued fraction for sqrt(29).
 * https://oeis.org/A010128
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9510, u16 n <= 9510, u32 n <= 9510, u64 n <= 9510,
 *            u128 n <= 9510, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic partial quotients of the continued fraction for sqrt(29).
   a(0) = 5, for n >= 1 : a(n) = period[(n-1) mod 5]
   with period = {2,1,1,2,10}. */
#include <stddef.h>
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)5U;
    static const seq_t period[5] = { (seq_t)2U, (seq_t)1U,
                                     (seq_t)1U, (seq_t)2U,
                                     (seq_t)10U };
    return period[(size_t)((n - 1U) % 5U)];
}
