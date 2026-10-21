/*
 * A230772  Number of primes in the half-open interval [n, 3*n/2).
 * https://oeis.org/A230772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes p with n ≤ p < 3·n/2 (half‑open interval
 * Range:     u8 n <= 4439, u16 n <= 6667, u32 n <= 6667, u64 n <= 6667,
 *            u128 n <= 6667, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* with offset 1 :  a(1) = 0, a(2) = 1, …) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

static idx_t sieve_limit = 0;	/* largest index covered so far */
static uint8_t *sieve = NULL;	/* 0 → composite, 1 → prime */

/* ensure the sieve covers all numbers up to 'limit' (inclusive) */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    free(sieve);
    sieve = (uint8_t *)calloc((size_t)limit + 1U, sizeof(uint8_t));
    if (!sieve)
        abort();                        /* out of memory */

    /* initialise to prime ; 0 and 1 are composite */
    for (idx_t i = 0; i <= limit; ++i)
        sieve[i] = 1;
    sieve[0] = sieve[1] = 0;

    /* simple Eratosthenes sieve */
    for (idx_t p = 2; p <= limit / p; ++p) {
        if (sieve[p]) {
            for (idx_t m = p * p; m <= limit; m += p)
                sieve[m] = 0;
        }
    }

    sieve_limit = limit;
}

/* Return the nth term of sequence A230772. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* defined only for n ≥ 1 */

    /* ceil(3*n/2) : end of the half‑open interval, exclusive */
    idx_t end = n + (n >> 1) + (n & 1); /* n + floor(n/2) + [n odd] */

    ensure_sieve(end);

    seq_t count = 0;
    for (idx_t i = n; i < end; ++i)
        if (sieve[i])
            ++count;

    return count;
}
