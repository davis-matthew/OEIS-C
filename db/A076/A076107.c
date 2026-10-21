/*
 * A076107  First of n consecutive integers whose sum is a positive n-th
 *          power, or 0 if no such integers exist.
 * https://oeis.org/A076107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p^n - n*(n-1)/2) / n where * p = product of all distinct
 *            odd primes dividing n. * If p == 1 and n != 1, the sequence
 *            value is 0 (no solution exists)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, product_of_odd_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/product_of_odd_primes.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, just return 0 */
        return (seq_t)0;

    /* Special case: n == 1, the only solution is a = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Determine the product of distinct odd primes dividing n */
    seq_t p = product_of_odd_primes(n);
    if (p == (seq_t)1)        /* no odd primes -> no solution */
        return (seq_t)0;

    /* Compute p^n using the provided big integer routine */
    seq_t pow = big_pow(p, (seq_t)n);

    /* Compute n*(n-1)/2 as seq_t */
    seq_t t = (seq_t)n * ((seq_t)n - 1U) / 2U;

    /* Check that (pow - t) is divisible by n */
    seq_t rem_pow = pow % (seq_t)n;
    seq_t rem_t   = t   % (seq_t)n;
    if (rem_pow != rem_t)
        return (seq_t)0;                /* no integer solution */

    return (pow - t) / (seq_t)n;
}
