/*
 * A152771  a(n) = sigma(n) - 2*d(n) + 1.
 * https://oeis.org/A152771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(n) - 2*d(n) + 1
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);                  /* Σ(n) */
    seq_t d = (seq_t)divisor_count_u64(n);  /* τ(n) */
    return s - (d << 1) + 1;                /* σ(n) - 2·τ(n) + 1 */
}
