/*
 * A073482  Largest prime factor of the n-th squarefree number.
 * https://oeis.org/A073482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime factor of the n-th squarefree number
 * Range:     u8 n <= 157, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

/* Test if a 64‑bit number is square‑free by trial division. */
static bool is_squarefree_uint64(uint64_t n)
{
    if (n <= 1U) return true;
    for (uint64_t p = 2U; p * p <= n; ++p) {
        if (n % p == 0U) {
            n /= p;                     /* remove one occurrence      */
            if (n % p == 0U)            /* second occurrence => false */
                return false;
        }
    }
    return true;
}

/* Return the largest prime factor of the n‑th squarefree integer. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset */

    uint64_t candidate = 1U;              /* integer to test          */
    idx_t    found     = 0U;              /* how many squarefrees seen*/
    seq_t    last_val  = 1U;              /* value for the last one    */

    while (found < n) {
        if (is_squarefree_uint64(candidate)) {
            last_val = (candidate <= 1U) ? (seq_t)1U
                                         : (seq_t)largest_prime_factor(candidate);
            ++found;
        }
        ++candidate;
    }
    return last_val;
}
