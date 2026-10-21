/*
 * A075473  n^n + n! - 2*(n+1)^(n-1).
 * https://oeis.org/A075473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n + n! - 2*(n+1)^(n-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/fact_upto.h"

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return 0;

    seq_t pow_nee  = big_pow((seq_t)n,          (seq_t)n);
    seq_t fact     = fact_upto(n);
    seq_t pow_n1   = big_pow((seq_t)(n + 1),    (seq_t)(n - 1));

    seq_t double_pow = pow_n1 + pow_n1;          /* 2 * (n+1)^(n-1) */
    seq_t result     = pow_nee + fact - double_pow;

    return result;
}
