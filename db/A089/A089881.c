/*
 * A089881  Number of primes <= n that are members of twin prime pairs.
 * https://oeis.org/A089881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of primes ≤ n that are members of a twin prime pair.
 * The function maintains a static sieve that is extended as needed.   */
#include <stdlib.h>
#include <stdint.h>

static unsigned char *prime_flags = NULL; /* 0: composite, 1: prime */
static idx_t sieve_limit = 0;             /* highest index for which flags are valid */

/* Extend the sieve to cover at least 'limit'.  Uses a standard
 * O(limit log log limit) sieve, re‑allocating the flag array.   */
static void
ensure_sieve(idx_t limit)
{
    if (prime_flags != NULL && limit <= sieve_limit)
        return;

    unsigned char *new_flags = (unsigned char *)malloc((size_t)(limit + 1));
    if (!new_flags)
        abort();        /* out of memory – should not happen on the test harness */

    /* Initialise all entries as prime; 0 and 1 are handled separately. */
    for (idx_t i = 0; i <= limit; ++i)
        new_flags[i] = 1;
    new_flags[0] = 0;
    new_flags[1] = 0;

    /* Classic sieve of Eratosthenes on the new range. */
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= limit; ++p) {
        if (new_flags[p]) {
            for (idx_t j = p * p; j <= limit; j += p)
                new_flags[j] = 0;
        }
    }

    free(prime_flags);
    prime_flags = new_flags;
    sieve_limit = limit;
}

/* a(n) = number of primes ≤ n that belong to a twin prime pair.   */
seq_t
a(idx_t n)
{
    if (n < 3)             /* 2 is never part of a twin pair in this sense */
        return 0;

    /* Build sieve up to n (we only need primes up to n). */
    ensure_sieve(n);

    seq_t count = 0;
    for (idx_t p = 2; p <= n; ++p) {
        if (!prime_flags[p])
            continue;

        int left  = (p >= 3)        && prime_flags[p - 2];
        int right = (p + 2 <= n)    && prime_flags[p + 2];
        if (left || right)
            ++count;
    }
    return count;
}
