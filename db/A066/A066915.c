/*
 * A066915  a(n) = n^phi(n) + 1.
 * https://oeis.org/A066915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^phi(n) + 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t phi_n  = phi(n);                /* Euler's totient of n   */
    seq_t base   = (seq_t)n;              /* cast index to seq_t    */
    seq_t pow_val = big_pow(base, phi_n); /* compute n^phi(n)        */
    return pow_val + 1;                   /* final result            */
}
