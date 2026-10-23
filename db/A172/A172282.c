/*
 * A172282  Squares of Bernoulli number denominators A027642.
 * https://oeis.org/A172282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (∏_{p prime, (p‑1)|n} p)², where a(0)=1, a(1)=4, and
 *            a(n)=1 for odd n>1 (Bernoulli number zero)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 35, u64 n <= 179, u128 n <= 539,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for OEIS A172282 */
seq_t a(idx_t n)
{
    /* Handle the trivial cases directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;
    if (n & 1u)    /* odd n > 1 */
        return (seq_t)1;

    /* For even n, compute the product of primes p ≤ n+1 such that (p‑1) | n */
    idx_t limit = n + 1;
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0;          /* allocation failure → 0 as safe fallback */

    /* Sieve of Eratosthenes */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    seq_t denom = (seq_t)1;
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;
        if ((n % (p - 1)) == 0)
            denom *= (seq_t)p;          /* multiply into denominator */
    }

    free(is_prime);
    return denom * denom;                /* return square of the denominator */
}
