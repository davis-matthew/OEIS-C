/*
 * A002246  a(1) = 3; for n > 1, a(n) = 4*phi(n); given a rational number r =
 *          p/q, where q>0, (p,q)=1, define its height to be max{|p|,q}; then
 *          a(n) = number of rational numbers of height n.
 * https://oeis.org/A002246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 3; for n > 1, a(n) = 4*phi(n), where phi is Euler's
 *            totient function
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/totient.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)3;
    return (seq_t)(4 * totient((uint64_t)n));
}
