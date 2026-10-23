/*
 * A161658  a(n) = the largest multiple of {the sum of the prime-factorization
 *          exponents of n} that is <= n.
 * https://oeis.org/A161658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (n mod A001222(n))
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Below the offset – return 0 */
    seq_t omega = bigomega(n);      /* A001222(n) – sum of exponents */
    if (omega == 0) return 0;       /* n = 1 */
    seq_t mod = ((seq_t)n) % omega;
    return ((seq_t)n) - mod;
}
