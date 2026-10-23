/*
 * A185141  a(n) = (n!)^(2*n).
 * https://oeis.org/A185141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)^(2*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n = 0 the formula gives 1^0 = 1, which is the correct first term. */
    /* Compute factorial(n). */
    seq_t fact = factorial(n);
    /* Exponent = 2 * n, promoted to seq_t. */
    seq_t exp = (seq_t)(2ULL * (seq_t)n);
    /* Compute (fact)^(exp). */
    return big_pow(fact, exp);
}
