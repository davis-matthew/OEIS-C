/*
 * A082139  A transform of binomial(n,5).
 * https://oeis.org/A082139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n-1) + 0^n/2) * binomial(n+5, n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 44, u128 n <= 102,
 *            big any n
 * Helpers:   binom_n_plus5_choose_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = binomial(n+5,5) * 2^(n-1)  for n ≥ 1,  a(0)=1 */

#include <stddef.h>
#include "../utils/binom_n_plus5_choose_5.h"

/* The binomial function is provided by the test harness (if needed). */
extern seq_t binomial(idx_t n, idx_t k);

/* Helper: compute 2^(exp) exactly (exp ≥ 0) */
static seq_t pow2(idx_t exp)
{
    seq_t val = 1;
    for (idx_t i = 0; i < exp; ++i)
        val *= (seq_t)2;
    return val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* a(0)=1 */
    /* For n ≥ 1: a(n) = C(n+5,5) * 2^(n-1) */
    seq_t binom = binom_n_plus5_choose_5(n);
    seq_t pow2n = pow2(n - 1);
    return binom * pow2n;
}
