/*
 * A066916  a(n) = n^phi(n) - 1.
 * https://oeis.org/A066916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^phi(n) - 1
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
    if (n == 0) /* Undefined for index 0, but return 0 to be safe */
        return (seq_t)0;
    seq_t phi_n   = phi(n);          /* phi(n) fits in seq_t */
    seq_t base    = (seq_t)n;        /* convert index to seq_t */
    seq_t result  = big_pow(base, phi_n); /* compute n^phi(n) */
    /* Subtract one */
    result = result - 1;
    return result;
}
