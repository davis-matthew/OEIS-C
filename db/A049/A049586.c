/*
 * A049586  a(n) is the GCD of the cototients (A051953) of n and n+1.
 * https://oeis.org/A049586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( A051953(n+1), A051953(n+2) )
 * Range:     u8 n <= 1497, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   calc_phi, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t and compute the two post‑incremented
       values that are needed for the cototients. */
    seq_t m1 = (seq_t)n + 1;   /* n+1   */
    seq_t m2 = (seq_t)n + 2;   /* n+2   */

    /* Compute φ for each of the two numbers. */
    seq_t phi1 = calc_phi(m1);
    seq_t phi2 = calc_phi(m2);

    /* Cototient is defined as m – φ(m). */
    seq_t cot1 = m1 - phi1;   /* cot(n+1) */
    seq_t cot2 = m2 - phi2;   /* cot(n+2) */

    /* Return the greatest common divisor of the two cototients. */
    return gcd(cot1, cot2);
}
