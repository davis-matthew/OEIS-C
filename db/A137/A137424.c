/*
 * A137424  a(n) = binomial(digsum(n), n mod digsum(n)).
 * https://oeis.org/A137424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = binomial(digsum(n), n mod digsum(n))
 * Range:     u8 n <= 28, u16 n <= 198, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n ≥ 1, digsum(n) cannot be 0 */
    seq_t dseq = digit_sum((seq_t)n);           /* decimal digit sum of n   */
    idx_t  d    = (idx_t)dseq;                  /* safe cast: d ≤ 9 * digits(n) */
    idx_t  k    = n % d;                        /* remainder modulo the digit sum */

    return binomial(d, k);                     /* binomial(d, k) in seq_t  */
}
