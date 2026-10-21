/*
 * A065515  Number of prime powers <= n.
 * https://oeis.org/A065515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = the number of prime powers ≤ n, including 1 Equivalent to
 *            a(n) = 1 + Σ_{k=1..⌊log₂ n⌋} π(⌊n¹Ꮤ/k⌋) (π = prime counting)
 * Range:     u8 n <= 1432, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for other indices. */
    if (n < 1) {
        return 0;
    }

    /* Allocate a byte array for the sieve.  We use char so that the
       compiler knows it is a pointer to a byte-sized type.  The cast
       is required for C++ compatibility, because malloc returns void*. */
    size_t sz = (size_t)n + 1;
    char *is_prime = (char *)malloc(sz);
    if (!is_prime) {
        /* Allocation failure – return 0 as a graceful fallback. */
        return 0;
    }

    /* Initialise all entries to “prime”.  Then clear 0 and 1. */
    for (size_t i = 0; i < sz; ++i) {
        is_prime[i] = 1;
    }
    if (sz > 0)  is_prime[0] = 0;
    if (sz > 1)  is_prime[1] = 0;

    /* Sieve of Eratosthenes: mark composites.  We must not cross
       the bounds of idx_t, therefore use p <= n / p. */
    for (idx_t p = 2; p <= n / p; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p) {
                is_prime[m] = 0;
            }
        }
    }

    /* Count prime powers.  The number 1 is a valid prime power (p⁰
       for any prime p) and is counted separately. */
    seq_t count = 1;          /* count the number 1 */

    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p]) {
            continue;
        }
        uint64_t power = p;
        while (power <= n) {
            ++count;
            if (power > n / p) {   /* next multiplication would exceed n */
                break;
            }
            power *= p;
        }
    }

    free(is_prime);
    return count;
}
