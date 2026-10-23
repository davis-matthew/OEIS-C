/*
 * A022934  Number of 2^m between e^n and e^(n+1).
 * https://oeis.org/A022934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)/log(2)) - floor(n/log(2))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Natural logarithm of 2, sufficient accuracy for the required n. */
static const double LOG2_VAL = 0.6931471805599453094172321214581765680755;

/* External linkage function as demanded by the spec. */
seq_t a(idx_t n)
{
    /* Compute floor(n / ln 2) and floor((n+1) / ln 2) using double division. */
    uint64_t f_n   = (uint64_t)((double)n / LOG2_VAL);
    uint64_t f_n1  = (uint64_t)((double)(n + 1) / LOG2_VAL);

    /* The difference is always 1 or 2. */
    return (seq_t)(f_n1 - f_n);
}
