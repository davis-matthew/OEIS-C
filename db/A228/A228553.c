/*
 * A228553  Sum of the products formed by multiplying together the smaller and
 *          larger parts of each Goldbach partition of 2n.
 * https://oeis.org/A228553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=2..n} [prime(i) && prime(2*n-i)] * i * (2*n-i)
 * Range:     u8 n <= 10, u16 n <= 89, u32 n <= 4619, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* calloc, free */
#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */
#include <stdbool.h>     /* bool */
#include <math.h>        /* sqrt */

/* -------------  Prime sieve infrastructure ------------- */

static unsigned char *prime_flags = NULL;  /* 0 = non‑prime, 1 = prime */
static idx_t          prime_limit = 0;     /* highest index flagged */

/* Expand the sieve to cover numbers up to `limit` (inclusive). */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;          /* already sufficient */

    /* Allocate new sieve array: cast needed for C++ compilation. */
    unsigned char *new_flags = (unsigned char *)calloc((size_t)limit + 1, sizeof(unsigned char));
    if (!new_flags) abort();                   /* allocation failure */

    /* Mark all numbers >=2 as prime initially. */
    for (idx_t i = 2; i <= limit; ++i)
        new_flags[i] = 1;

    /* Classic sieve of Eratosthenes. */
    idx_t sqrtL = (idx_t)sqrt((double)limit);
    for (idx_t p = 2; p <= sqrtL; ++p) {
        if (new_flags[p]) {
            for (idx_t q = p * p; q <= limit; q += p)
                new_flags[q] = 0;
        }
    }

    /* Replace old sieve and update limit. */
    free(prime_flags);
    prime_flags = new_flags;
    prime_limit = limit;
}

/* -------------  Sequence implementation ------------- */

/* Return a(n) for n >= 1.  Undefined for n < 1 (returns 0). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    idx_t limit = n + n;          /* 2 * n */
    ensure_primes(limit);

    seq_t sum = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (prime_flags[i] && prime_flags[limit - i]) {
            sum += (seq_t)i * (seq_t)(limit - i);
        }
    }

    return sum;
}
