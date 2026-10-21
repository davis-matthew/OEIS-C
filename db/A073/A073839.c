/*
 * A073839  Sum of the composite numbers between n and 2n (both inclusive).
 * https://oeis.org/A073839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all composite integers k with n ≤ k ≤ 2n
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Static sieve: 0 → probably prime, 1 → composite (or 0/1). */
static uint8_t *sieve = NULL;
static idx_t sieve_limit = 0;

/* Extend the sieve up to the supplied limit (inclusive). */
static void extend_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    size_t sz = (size_t)(limit + 1);
    uint8_t *new_sieve = (uint8_t *)malloc(sz);
    if (!new_sieve)
        return; /* Fallback: caller will fail gracefully. */

    memset(new_sieve, 0, sz);        /* Start with all primes. */
    if (sz > 0) new_sieve[0] = new_sieve[1] = 1; /* 0 and 1 are not primes. */

    for (idx_t i = 2; (unsigned __int128)i * i <= limit; ++i) {
        if (new_sieve[i] == 0) { /* i is prime. */
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = 1; /* Mark composite. */
        }
    }

    if (sieve)
        free(sieve);
    sieve = new_sieve;
    sieve_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* out of offset, return 0 */

    extend_sieve(n * 2);
    seq_t sum = 0;

    for (idx_t k = n; k <= n * 2; ++k) {
        if (k > 1 && sieve[k]) /* composite */
            sum += (seq_t)k;
    }
    return sum;
}
