/*
 * A096457  If n is prime replace n with the next prime.
 * https://oeis.org/A096457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is composite or 1; otherwise a(n) = the next prime
 *            after n
 * Range:     u8 n <= 250, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static bool initialized = false;
static bool *prime_flags = NULL;
#define SIEVE_LIMIT 20000U

/* Initialize sieve of Eratosthenes up to SIEVE_LIMIT. */
static void init_primes(void) {
    size_t sz = (size_t)SIEVE_LIMIT + 1;
    bool *flags = (bool *)malloc(sz * sizeof(bool));
    for (size_t i = 0; i < sz; ++i)
        flags[i] = true;
    if (sz > 0) flags[0] = false;
    if (sz > 1) flags[1] = false;

    for (idx_t p = 2; (idx_t)p * p <= SIEVE_LIMIT; ++p) {
        if (flags[(size_t)p]) {
            for (idx_t j = p * p; j <= SIEVE_LIMIT; j += p)
                flags[(size_t)j] = false;
        }
    }
    prime_flags = flags;
    initialized = true;
}

seq_t a(idx_t n) {
    if (!initialized)
        init_primes();

    if (n <= 1U)      /* a(1) = 1 */
        return (seq_t)n;

    /* If n is not prime, return n itself. */
    if (!prime_flags[(size_t)n])
        return (seq_t)n;

    /* Find next prime after n. */
    idx_t nxt = n + 1U;
    while (nxt <= SIEVE_LIMIT && !prime_flags[(size_t)nxt])
        ++nxt;

    /* nxt will always be found within the bound for required n. */
    return (seq_t)nxt;
}
