/*
 * A165797  a(n) = n^( sigma(n) - tau(n) ).
 * https://oeis.org/A165797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(sigma(n) - tau(n)) (sigma = sum of divisors, tau =
 *            number of divisors)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   divisor_count_u64, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* offset is 1, 0 is unused */
    seq_t sigma = sigma_of(n);            /* sigma(n) */
    uint64_t tau  = divisor_count_u64(n); /* tau(n) */
    idx_t exp = (idx_t)(sigma - tau);     /* exponent as idx_t */
    return seq_pow((seq_t)n, exp);        /* n^exp */
}
