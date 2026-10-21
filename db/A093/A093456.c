/*
 * A093456  Product of composite numbers among next n numbers.
 * https://oeis.org/A093456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = product of all composite numbers in * [n*(n-1)/2 + 1 ,
 *            n*(n+1)/2] (inclusive). * For n = 1, the product is defined to
 *            be 1. * * Implementation details: * • A simple sieve of
 *            Eratosthenes is kept in a static bit‑array. * • The sieve is
 *            expanded lazily to cover the required maximum * value f...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static sieve storage.  prime_sieve[i] == 1  <=>  i is prime. */
static uint8_t *prime_sieve = NULL;
static idx_t sieve_limit = 0;

/* --- Prime sieve ----------------------------------------------------- */
static void ensure_primes(idx_t max_val)
{
    if (sieve_limit >= max_val)
        return;                     /* current sieve is sufficient */

    /* Allocate new buffer */
    uint8_t *buf = (uint8_t *)calloc((size_t)max_val + 1, 1);
    if (!buf)                         /* allocation failed – abort */
        return;

    /* Mark every number as prime initially, then clear 0 and 1. */
    for (idx_t i = 0; i <= max_val; ++i)
        buf[i] = 1;
    if (max_val >= 0) buf[0] = 0;
    if (max_val >= 1) buf[1] = 0;

    /* Eratosthenes: clear composites */
    for (idx_t p = 2; p * p <= max_val; ++p)
    {
        if (!buf[p])
            continue;
        for (idx_t mult = p * p; mult <= max_val; mult += p)
            buf[mult] = 0;
    }

    /* Replace old sieve */
    free(prime_sieve);
    prime_sieve = buf;
    sieve_limit = max_val;
}

/* --- Sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset is 1 – a(0) is irrelevant */
        return (seq_t)0;

    /* Determine the interval that has to be examined. */
    idx_t lower = n * (n - 1) / 2 + 1;
    idx_t upper = n * (n + 1) / 2;

    /* Make sure primes up to 'upper' are available. */
    ensure_primes(upper);

    seq_t product = (seq_t)1;   /* product of composites */

    for (idx_t val = lower; val <= upper; ++val)
    {
        /* 1 is neither prime nor composite. */
        if (val == 1)
            continue;
        /* Skip primes. */
        if (prime_sieve[val])
            continue;
        /* Composite number – multiply. */
        product *= (seq_t)val;
    }
    return product;
}
