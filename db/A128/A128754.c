/*
 * A128754  Permutation of positive integers obtained by swapping n-th natural
 *          number with the (n-g)-th sequentially, where
 *          g=prime(n+1)-prime(n)-1.
 * https://oeis.org/A128754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permutation of the natural numbers obtained by *
 *            sequentially swapping the element at position n with the element
 *            * at position n-g, where g = prime(n+1)-prime(n)-1. * a(1) is
 *            the first term of the sequence
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Helper to generate the first 'need' primes.
 * 'primes' must be a pointer to a uint32_t* that will receive a
 * malloc'ed array of at least 'need' elements.  The caller must
 * free the array.  */
static void generate_first_primes(size_t need, uint32_t **primes)
{
    /* Rough upper bound for prime(need).  For n>=6:
     * p_n < n (ln n + ln ln n).  Add a small safety margin.
     */
    if (need < 6) need = 6;
    double n = (double)need;
    size_t limit = (size_t)(n * (log(n) + log(log(n))) + 3.0);

    /* If bound is too tight, keep doubling until we have enough primes. */
    while (true) {
        /* Allocate sieve */
        bool *sieve = (bool *)calloc(limit + 1, sizeof(bool));
        if (!sieve) return; /* allocation failed – let caller handle */
        for (size_t i = 2; i <= limit; ++i) sieve[i] = true;

        size_t sqrt_l = (size_t)sqrt((double)limit);
        for (size_t p = 2; p <= sqrt_l; ++p) {
            if (sieve[p]) {
                for (size_t k = p * p; k <= limit; k += p)
                    sieve[k] = false;
            }
        }
        /* Count primes to see if we have enough */
        size_t count = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (sieve[i]) ++count;

        if (count >= need) {
            /* Build array */
            *primes = (uint32_t *)malloc(sizeof(uint32_t) * need);
            if (*primes) {
                size_t idx = 0;
                for (size_t i = 2; i <= limit && idx < need; ++i)
                    if (sieve[i]) (*primes)[idx++] = (uint32_t)i;
            }
            free(sieve);
            break;
        }
        free(sieve);
        /* Increase limit and try again */
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have enough indices to be certain the final value
     * at position n is stable.  A margin of 200 is generous for
     * n up to ~10000 (prime gaps ~ < 40).  */
    idx_t M = n + 200;

    /* We need primes up to (M+1)-th prime (index starting at 1). */
    size_t prime_count = (size_t)(M + 2); /* primes[0] = prime(1) */
    uint32_t *primes = NULL;
    generate_first_primes(prime_count, &primes);
    if (!primes) return 0; /* in case of memory failure */

    /* Allocate array for the permutation (1‑based). */
    seq_t *perm = (seq_t *)calloc((size_t)(M + 1), sizeof(seq_t));
    if (!perm) {
        free(primes);
        return 0;
    }
    for (size_t i = 1; i <= (size_t)M; ++i)
        perm[i] = (seq_t)i;

    /* Perform the sequence of swaps. */
    for (size_t i = 1; i <= (size_t)M; ++i) {
        /* primes[i] = prime(i+1), primes[i-1] = prime(i) */
        idx_t g = (idx_t)primes[i] - (idx_t)primes[i - 1] - 1;
        idx_t j = (idx_t)i - g;              /* guaranteed >= 1 */
        seq_t tmp = perm[i];
        perm[i] = perm[j];
        perm[j] = tmp;
    }

    seq_t result = perm[(size_t)n];
    free(perm);
    free(primes);
    return result;
}
