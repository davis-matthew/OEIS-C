/*
 * A157672  Number of unordered factorizations of n! into two distinct proper
 *          factors.
 * https://oeis.org/A157672
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A027423(n)/2 - 1 (where A027423(n)=tau(n!))
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;  /* offset is 2, values below are irrelevant */

    /* Build a prime sieve up to n */
    size_t limit = (size_t)n;
    char *is_prime = (char*)malloc((limit + 1) * sizeof(char));
    if (!is_prime) return (seq_t)0;

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i) {
                is_prime[j] = 0;
            }
        }
    }

    /* Compute tau(n!) = product over primes (e_p + 1) */
    seq_t tau = 1;
    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            idx_t p = (idx_t)i;
            idx_t e = 0;
            idx_t t = n;
            while (t) {
                t /= p;
                e += t;
            }
            tau *= (seq_t)(e + 1);
        }
    }

    free(is_prime);

    /* a(n) = tau/2 - 1 */
    seq_t ans = tau / 2;
    ans -= 1;
    return ans;
}
