/*
 * A073046  Write 2*n = p + q (p, q prime), p*q minimal; then a(n) = p*q.
 * https://oeis.org/A073046
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   minimal product of two primes summing to 2*n a(n) = p * q where
 *            p + q = 2*n and p,q are prime, and p is minimal. For n >= 2 this
 *            yields the OEIS sequence A073046
 * Range:     u8 n <= 26, u16 n <= 495, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static size_t sieve_limit = 0;
static bool *sieve = NULL;

/* Ensure the sieve contains primes up to 'limit'. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Allocate a fresh sieve array. */
    bool *new_sieve = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!new_sieve) return; /* allocation failed, leave empty */

    for (size_t i = 0; i <= (size_t)limit; ++i)
        new_sieve[i] = true;
    if (limit >= 0) new_sieve[0] = false;
    if (limit >= 1) new_sieve[1] = false;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (new_sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_sieve[j] = false;
        }
    }

    /* Replace the old sieve. */
    if (sieve) free(sieve);
    sieve = new_sieve;
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined below offset */

    idx_t sum = 2 * n;                    /* target sum of the two primes */
    ensure_sieve(sum);                    /* make sure we know all primes up to sum */

    for (idx_t p = 2; p <= sum / 2; ++p) {
        if (sieve[p] && sieve[sum - p]) {
            return (seq_t)p * (seq_t)(sum - p);
        }
    }

    /* Under Goldbach's conjecture this situation should never occur. */
    return 0;
}
