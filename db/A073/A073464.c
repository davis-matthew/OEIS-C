/*
 * A073464  a(n) = phi(n) mod PrimePi(n).
 * https://oeis.org/A073464
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(n) mod pi(n) (offset 2)
 * Range:     u8 n <= 267, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* For indices below the official offset, return 0 as a placeholder. */
    if (n < 2)
        return (seq_t)0;

    seq_t phi_val = phi(n);          /* Euler's totient of n */
    seq_t pi_val  = compute_pi(n);   /* Prime counting function π(n) */

    /* π(n) ≥ 1 for n ≥ 2, so the modulo operation is safe. */
    return phi_val % pi_val;
}
