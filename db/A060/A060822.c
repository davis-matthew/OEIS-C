/*
 * A060822  a(n) = prime(n) + n^3 + n^2 + 4n - 1.
 * https://oeis.org/A060822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, dumb
 * Formula:   a(n) = prime(n) + n^3 + n^2 + 4*n - 1
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static prime table to be built lazily */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t prime_cap = 0;

    /* grow prime table until we have at least n primes */
    if (prime_count < n) {
        if (primes == NULL) {
            prime_cap = 256;
            primes = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        }
        uint64_t candidate = 2;
        if (prime_count > 0)
            candidate = primes[prime_count - 1] + 1;

        while (prime_count < n) {
            int is_prime = 1;
            for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= candidate; ++i) {
                if (candidate % primes[i] == 0) {
                    is_prime = 0;
                    break;
                }
            }
            if (is_prime) {
                if (prime_count == prime_cap) {
                    prime_cap *= 2;
                    primes = (uint64_t *)realloc(primes, prime_cap * sizeof(uint64_t));
                }
                primes[prime_count++] = candidate;
            }
            candidate++;
        }
    }

    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)primes[n - 1];   /* prime(n) in 0‑based index */
    term += nn * nn * nn;               /* n^3 */
    term += nn * nn;                    /* n^2 */
    term += (seq_t)4 * nn;              /* 4n */
    term -= (seq_t)1;                   /* -1 */

    return term;
}
