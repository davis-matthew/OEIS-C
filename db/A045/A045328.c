/*
 * A045328  Primes congruent to {0, 1, 2, 3, 6} (mod 7).
 * https://oeis.org/A045328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Compute the n‑th prime p such that p ≡ 0,1,2,3 or 6 (mod 7).
 * The function constructs a sieve up to an adaptive bound until the
 * required prime is found.  It works for any seq_t (plain integer or
 * arbitrary-precision) and any idx_t (uint64_t).
 */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The function assumes n >= 1.  No special handling for n == 0. */
    /* Initial bound: use a generous linear estimate that works for
       n up to at least 10000.  If not enough primes are found the bound
       is doubled until the condition is satisfied. */
    uint64_t bound = n * 20ULL + 100ULL;   /* start with a safe margin */

    while (1) {
        /* Allocate the sieve array.  Cast explicitly for C++ compatibility. */
        unsigned char *is_prime =
            (unsigned char *)malloc((bound + 1ULL) * sizeof(unsigned char));
        if (!is_prime) return (seq_t)0;   /* out of memory, signal failure */

        /* Initialize all entries as potential primes. */
        for (uint64_t i = 0; i <= bound; ++i)
            is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        /* Classic Eratosthenes sieve. */
        for (uint64_t i = 2; i * i <= bound; ++i) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= bound; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Scan the sieve for primes in the desired residue classes. */
        uint64_t found = 0;
        for (uint64_t p = 2; p <= bound; ++p) {
            if (!is_prime[p]) continue;
            uint64_t r = p % 7ULL;
            if (r == 0ULL || r == 1ULL || r == 2ULL ||
                r == 3ULL || r == 6ULL) {
                ++found;
                if (found == n) {
                    seq_t ans = (seq_t)p;
                    free(is_prime);
                    return ans;
                }
            }
        }

        /* Not enough primes found – increase the bound and retry. */
        free(is_prime);
        bound = bound * 2ULL + 100ULL;
    }
}
