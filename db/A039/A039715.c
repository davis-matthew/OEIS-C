/*
 * A039715  Primes modulo 17.
 * https://oeis.org/A039715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod 17
 * Range:     u8 n <= 54, u16 n <= 7041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle requests with offset < 1 */
    if (n < 1) return (seq_t)0;

    /* Static state to maintain primes */
    static seq_t *primes = NULL;
    static idx_t prime_count = 0;
    static uint64_t last_checked = 1;
    static size_t cap = 0;

    if (prime_count < n) {
        /* Allocate the prime buffer if needed */
        if (!primes) {
            cap = 32;
            primes = (seq_t*)malloc(cap * sizeof(seq_t));
        }

        uint64_t cand;
        for (cand = last_checked + 1; prime_count < n; ++cand) {
            /* Check if candidate is prime by trial division */
            int isprime = 1;
            for (idx_t i = 0; i < prime_count &&
                    (uint64_t)primes[i] * (uint64_t)primes[i] <= cand; ++i) {
                if (cand % (uint64_t)primes[i] == 0) {
                    isprime = 0;
                    break;
                }
            }
            if (isprime) {
                /* Grow the prime array if necessary */
                if (prime_count == cap) {
                    cap <<= 1;
                    primes = (seq_t*)realloc(primes, cap * sizeof(seq_t));
                }
                primes[prime_count++] = (seq_t)cand;
            }
        }
        last_checked = cand - 1;
    }

    /* Return the n-th prime modulo 17 */
    seq_t p = primes[n - 1];
    uint64_t r = (uint64_t)p % 17ULL;
    return (seq_t)r;
}
