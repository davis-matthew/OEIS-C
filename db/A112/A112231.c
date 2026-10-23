/*
 * A112231  Repeat each prime in the sequence of natural numbers.
 * https://oeis.org/A112231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest m such that m + π(m) ≥ n, where π(m) counts
 *            the primes ≤ m (each prime contributes an extra occurrence).
 *            This implements the sequence “Repeat each prime in the sequence
 *            of natural numbers”
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static idx_t sieve_limit = 0;
static uint8_t *prime_flags = NULL;

/* Build a simple Eratosthenes sieve up to |limit|. */
static void compute_sieve(idx_t limit)
{
    /* Allocate and initialize prime flags to 0. */
    size_t size = (size_t)limit + 1;
    uint8_t *flags = (uint8_t *)calloc(size, sizeof(uint8_t));
    if (!flags) {           /* allocation failed – give up  */
        return;
    }

    /* Mark numbers ≥ 2 as potential primes. */
    if (size > 2) {
        for (idx_t i = 2; i <= limit; ++i) {
            flags[i] = 1;
        }
    }

    /* Eratosthenes sieve. */
    for (idx_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                flags[j] = 0;
            }
        }
    }

    /* Replace any previously allocated flag set. */
    free(prime_flags);
    prime_flags = flags;
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)         /* offset starts at 1; unused case */
        return (seq_t)0;

    /* Ensure the sieve reaches at least n. */
    if (n > sieve_limit) {
        compute_sieve(n);
    }

    /* Scan m=1…n, keeping a running count of primes. */
    idx_t pi = 0;              /* π(m) */
    for (idx_t m = 1; m <= n; ++m) {
        if (m >= 2 && prime_flags[m]) {
            ++pi;
        }
        if (m + pi >= n) {
            return (seq_t)m;
        }
    }

    /* The loop should always return before reaching this line. */
    return (seq_t)n;
}
