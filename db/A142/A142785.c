/*
 * A142785  Primes congruent to 58 mod 59.
 * https://oeis.org/A142785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A142785: primes congruent to 58 mod 59
 *
 * a(n) = the n-th prime p such that p % 59 == 58  (offset 1).
 *
 * The routine estimates an upper bound, sieves up to that bound,
 * counts the qualifying primes until the n-th one is reached,
 * and doubles the bound if needed.  All arithmetic uses
 * only integer operations on the supplied types (seq_t, idx_t).
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 1.  Return 0 for safety. */
    if (n < 1)
        return (seq_t)0;

    /* Rough upper bound estimate: 20 * 59 * n.  This is plenty for
     * n up to at least 10 000, and we will double if it turns out to
     * be insufficient. */
    idx_t bound = n * (idx_t)20 * (idx_t)59;
    if (bound < 1000)
        bound = 1000;

    while (1) {
        /* Allocate a simple boolean array for the sieve. */
        uint8_t *is_prime = (uint8_t *)calloc(bound + 1, sizeof(uint8_t));
        if (!is_prime)
            return (seq_t)0;          /* allocation failure */

        /* Initially mark all numbers >= 2 as prime. */
        for (idx_t i = 2; i <= bound; ++i)
            is_prime[i] = 1;

        /* Eratosthenes sieve. */
        for (idx_t p = 2; p * p <= bound; ++p) {
            if (is_prime[p]) {
                for (idx_t q = p * p; q <= bound; q += p)
                    is_prime[q] = 0;
            }
        }

        /* Find the n-th prime that is ≡ 58 (mod 59). */
        idx_t count = 0;
        seq_t result = 0;
        for (idx_t i = 2; i <= bound; ++i) {
            if (is_prime[i] && (i % 59) == 58) {
                ++count;
                if (count == n) {
                    result = (seq_t)i;
                    break;
                }
            }
        }

        free(is_prime);

        if (result != 0)
            return result;            /* success */

        /* Not enough primes found – double the bound and try again. */
        bound <<= 1;
    }
}
