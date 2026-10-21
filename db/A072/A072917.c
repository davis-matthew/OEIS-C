/*
 * A072917  a(n) = p(n) - phi(n), where p(n) is the least prime greater than
 *          phi(n).
 * https://oeis.org/A072917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p(n) – φ(n) where p(n) is the least prime greater than
 *            φ(n)
 * Range:     u8 n <= 420, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utilities phi() and next_prime() */

#include "../utils/phi.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* compute Euler's totient */
    seq_t phi_val = phi(n);

    /* find the least prime > phi_val */
    uint64_t next = next_prime((uint64_t)phi_val);

    /* result is the difference */
    return (seq_t)next - phi_val;
}
