/*
 * A062356  a(n) = floor(n/phi(n)).
 * https://oeis.org/A062356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = floor(n / phi(n))
 * Range:     u8 n <= 1901, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    // n ≥ 1 as per the specification
    seq_t nn = (seq_t)n;
    seq_t ph = phi(n);   /* phi(n) returns seq_t */
    return nn / ph;      /* integer division truncates → floor */
}
