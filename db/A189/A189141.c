/*
 * A189141  Zero-one sequence based on the primes: a(A000040(k))=a(k);
 *          a(A002808(k))=1-a(k), a(1)=0.
 * https://oeis.org/A189141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1. For n > 1: let π(n) be the number of primes ≤ n. If n
 *            is prime → a(n) = a(π(n)). If n is composite → a(n) = 1 – a(n –
 *            π(n))
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n<1, return 0 */
    size_t N = (size_t)n;
    size_t new_cap = N + 1;               /* indices 0..N inclusive */

    /* Static caches */
    static seq_t  *seq = NULL;
    static uint8_t *isprime = NULL;
    static size_t  cache_size = 0;

    if (new_cap > cache_size) {
        /* grow cache */
        cache_size = new_cap;
        seq = (seq_t*)realloc(seq, cache_size * sizeof(seq_t));
        isprime = (uint8_t*)realloc(isprime, cache_size * sizeof(uint8_t));

        /* initialise sieve */
        for (size_t i = 0; i < cache_size; ++i)
            isprime[i] = 1;
        if (cache_size > 0) isprime[0] = 0;
        if (cache_size > 1) isprime[1] = 0;
        for (size_t i = 2; i * i < cache_size; ++i) if (isprime[i]) {
            for (size_t j = i * i; j < cache_size; j += i)
                isprime[j] = 0;
        }

        /* compute sequence values */
        seq[1] = 1;                     /* a(1) = 1 */
        size_t prime_cnt = 0;           /* number of primes < current i */
        for (size_t i = 2; i < cache_size; ++i) {
            if (isprime[i]) {
                size_t pi_i = prime_cnt + 1;     /* π(i) */
                seq[i] = seq[pi_i];
                prime_cnt = pi_i;               /* update for next i */
            } else {
                size_t k = i - prime_cnt;        /* composite index */
                seq[i] = 1 - seq[k];
            }
        }
    }
    return seq[N];
}
