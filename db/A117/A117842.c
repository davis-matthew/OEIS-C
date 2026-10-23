/*
 * A117842  Partial sum of smallest prime >= n (A007918).
 * https://oeis.org/A117842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{i=0}^{n} nextprime(i) where nextprime(i) is the
 *            smallest prime ≥ i (A007918)
 * Range:     u8 n <= 21, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* choose sieve limit large enough to contain the next prime after n */
    idx_t limit = n + 500;
    if (limit < 2) limit = 2;

    /* allocate array of bools for primality */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return 0;          /* out‑of‑memory fallback */
    memset(is_prime, true, (limit + 1) * sizeof(bool));
    is_prime[0] = is_prime[1] = false;

    /* simple Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                is_prime[j] = false;
        }
    }

    /* array that holds the next prime ≥ i for all i <= limit */
    seq_t *next_prime = (seq_t *)malloc((limit + 1) * sizeof(seq_t));
    if (!next_prime) { free(is_prime); return 0; }

    seq_t next = 0;
    for (idx_t i = limit; ; --i) {
        if (is_prime[i]) next = (seq_t)i;
        next_prime[i] = next;
        if (i == 0) break;
    }

    /* compute the partial sum up to n */
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i)
        sum += next_prime[i];

    free(is_prime);
    free(next_prime);
    return sum;
}
