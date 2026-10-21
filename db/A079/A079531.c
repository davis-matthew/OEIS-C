/*
 * A079531  a(n) = phi(n) - ceiling(n^(2/3)).
 * https://oeis.org/A079531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = phi(n) - ceil(n^(2/3))
 * Range:     i8 n <= 161, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   ceil_pow_two_over_three, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the totient function from utils/phi.h and an integer cube‑root routine. */
#include "../utils/phi.h"
#include "../utils/ceil_pow_two_over_three.h"

seq_t a(idx_t n)
{
    seq_t phi_n = phi(n);
    seq_t ceil23 = ceil_pow_two_over_three(n);
    return phi_n - ceil23;
}
