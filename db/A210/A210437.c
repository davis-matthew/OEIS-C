/*
 * A210437  Greatest prime factor of reversal of digits of n.
 * https://oeis.org/A210437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = gpf(reverse(n)) where reverse(n) is the decimal reversal
 *            of n and gpf(x) is the greatest prime factor of x (with gpf(1) =
 *            1)
 * Range:     u8 n <= 103, u16 n <= 9872, u32 n <= 9872, u64 n <= 9872,
 *            u128 n <= 9872, big any n
 * Helpers:   largest_prime_factor, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* n is >= 1 */
    idx_t rev_n = rev(n);                 /* decimal reversal of n */
    uint64_t rev_val = (uint64_t) rev_n;   /* cast to 64‑bit integer for the helper */
    uint64_t gpf_val = largest_prime_factor(rev_val);
    return (seq_t) gpf_val;
}
