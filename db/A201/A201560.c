/*
 * A201560  a(n) = (Sum(m^(n-1), m=1..n-1) + 1) modulo n.
 * https://oeis.org/A201560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∑_{m=1}^{n-1} m^(n-1) + 1) mod n, a(1)=0
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assume seq_t and idx_t are defined by the build.
   The helper mod_pow is provided in ../utils/mod_pow.h. */
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)           /* a(1) = 0 */
        return (seq_t)0;

    seq_t mod  = (seq_t)n;    /* modulus */
    seq_t sum  = (seq_t)0;

    /* Compute the sum, reducing after each term to keep the
       intermediate numeric type safe. */
    for (idx_t m = 1; m < n; ++m) {
        seq_t term = mod_pow((seq_t)m, n - 1, mod);
        sum = (seq_t)(((unsigned int)sum + (unsigned int)term) % (unsigned int)mod);
    }

    /* Add the final +1 and reduce modulo n. */
    sum = (seq_t)(((unsigned int)sum + 1U) % (unsigned int)mod);
    return sum;
}
