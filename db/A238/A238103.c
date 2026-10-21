/*
 * A238103  Product of composite numbers in range n+1 to 2n, exclusive of
 *          endpoints.
 * https://oeis.org/A238103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = product of composite numbers in the interval (n, 2n) exclusive endpoints */

#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve data kept between calls */
static bool *prime_flags = NULL;
static idx_t prime_limit = 0;

/* Ensure prime information known up to max_limit */
static void
init_primes(idx_t max_limit)
{
    if (max_limit <= prime_limit)
        return;                      /* already computed */

    size_t sz = (size_t)max_limit + 1;
    bool *flags = (bool *)calloc(sz, sizeof(bool));
    if (!flags)                        /* allocation failure – fall back to no primes */
        return;

    /* Mark everything >=2 as prime initially */
    for (size_t i = 2; i < sz; ++i)
        flags[i] = true;

    /* Sieve of Eratosthenes */
    for (size_t p = 2; (size_t)p * p <= max_limit; ++p) {
        if (!flags[p])
            continue;
        for (size_t q = (size_t)p * p; q < sz; q += p)
            flags[q] = false;
    }

    free(prime_flags);
    prime_flags = flags;
    prime_limit = max_limit;
}

/* Prime test using the sieve cache */
static inline bool
is_prime(idx_t x)
{
    if (x < 2 || x > prime_limit)
        return false;
    return prime_flags[x];
}

/* Compute a(n) as the product of composite numbers in (n,2n) */
seq_t
a(idx_t n)
{
    idx_t upper = 2 * n;               /* upper exclusive endpoint */
    init_primes(upper);                /* ensure primes up to 2n */

    seq_t prod = 1;
    for (idx_t k = n + 2; k <= upper - 1; ++k) {
        if (!is_prime(k))              /* composite */
            prod *= (seq_t)k;          /* multiply into result */
    }
    return prod;
}
