/*
 * A100371  a(n) = 2^phi(n) - 1 = A066781(n) - 1.
 * https://oeis.org/A100371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^phi(n) - 1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   compute_phi, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_phi.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* phi(n) */
    seq_t ph = compute_phi((uint64_t)n);

    /* 2^phi(n) */
    seq_t val = pow2((idx_t)ph);

    /* subtract one */
    return val - (seq_t)1;
}
