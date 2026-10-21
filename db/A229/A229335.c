/*
 * A229335  Sum of sums of elements of subsets of divisors of n.
 * https://oeis.org/A229335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n) * 2^(tau(n)-1) (OEIS A229335)
 * Range:     u8 n <= 11, u16 n <= 59, u32 n <= 359, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64, pow2, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                /* size_t */
#include "../utils/sigma_of.h"     /* seq_t sigma_of(idx_t n); */
#include "../utils/divisor_count_u64.h"  /* size_t divisor_count_u64(uint64_t n); */
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* sigma(n): sum of divisors */
    seq_t sigma = sigma_of(n);

    /* tau(n): number of divisors */
    idx_t tau = (idx_t)divisor_count_u64(n);

    /* 2^(tau-1) */
    seq_t pow_val = pow2(tau - 1);

    /* product */
    return sigma * pow_val;
}
