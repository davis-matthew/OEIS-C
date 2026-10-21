/*
 * A093354  Number of prime triples (p,q,r) with p < q < r <= n and q - p = r
 *          - q.
 * https://oeis.org/A093354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A093354: Count of prime triples (p,q,r) with p<q<r≤n and q‑p = r‑q. */
/* The function generates all primes up to n and counts 3‑term arithmetic
   progressions among them. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For small n there are no triples. */
    if (n < 3)
        return (seq_t)0;

    /* Convert n to a movable size_t value for array handling. */
    size_t N = (size_t)n;

    /* --- Sieve of Eratosthenes ------------------------------------ */
    bool *is_prime = (bool *)calloc(N + 1, sizeof(bool));
    if (!is_prime)
        return (seq_t)0;                 /* allocation failure → 0 */

    for (size_t i = 2; i <= N; ++i)
        is_prime[i] = true;

    for (size_t p = 2; p * p <= N; ++p) {
        if (is_prime[p]) {
            for (size_t j = p * p; j <= N; j += p)
                is_prime[j] = false;
        }
    }

    /* --- Build list of primes ------------------------------------- */
    uint64_t *primes = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        return (seq_t)0;
    }

    size_t pcount = 0;
    for (size_t i = 2; i <= N; ++i) {
        if (is_prime[i])
            primes[pcount++] = (uint64_t)i;
    }

    /* --- Count prime triples -------------------------------------- */
    seq_t count = 0;
    for (size_t i = 0; i < pcount; ++i) {
        uint64_t p = primes[i];
        for (size_t j = i + 1; j < pcount; ++j) {
            uint64_t q = primes[j];
            uint64_t d = q - p;
            uint64_t r = q + d;
            if (r > n)
                break;                          /* further q only increase r */
            if (is_prime[(size_t)r])
                ++count;
        }
    }

    free(primes);
    free(is_prime);
    return count;
}
