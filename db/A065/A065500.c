/*
 * A065500  Number of distinct functions from a set with n^n elements to
 *          itself that can be defined naturally (in n) by typed
 *          lambda-calculus expressions.
 * https://oeis.org/A065500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = lcm(1..n) + n - 1, a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the least common multiple of all integers 1..n
   (using prime powers) and add n-1.
   For n == 0 the value is defined to be 1. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Quick handling of n == 1 */
    if (n == (idx_t)1)
        return (seq_t)1;   /* lcm(1)=1 and n-1=0 */

    /* Allocate a temporary bitmask for a simple sieve */
    size_t sz = (size_t)n + 1;
    bool *is_prime = (bool *)malloc(sz);
    if (!is_prime)
        return (seq_t)0;   /* allocation failed: return 0 as safe fallback */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= n; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    /* Compute the lcm as the product of the maximal prime powers ≤ n */
    seq_t lcm = (seq_t)1;
    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime[p]) {
            idx_t pow = p;
            while (pow * p <= n)
                pow *= p;
            lcm *= (seq_t)pow;
        }
    }

    free(is_prime);

    /* Final formula: a(n) = lcm(1..n) + n - 1 */
    seq_t result = lcm + (seq_t)(n - 1);
    return result;
}
