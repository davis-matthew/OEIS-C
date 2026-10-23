/*
 * A102648  a(n) is the number of primes p between 0 and 99 such that 100*n+p
 *          is also prime.
 * https://oeis.org/A102648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A102648
   a(n) = number of primes p < 100 such that 100*n + p is prime
   Formula: a(n) = | { p prime < 100 : is_prime(100*n + p) } |
*/

/* Include needed standard headers. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- List of primes less than 100 ---- */
static const uint8_t primes_lt100[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
static const size_t primes_cnt = sizeof(primes_lt100) / sizeof(primes_lt100[0]);

/* ---- Sieve data ---- */
static size_t sieve_cap = 0;  /* number of usable indices (0 .. sieve_cap-1) */
static bool *sieve = NULL;

/* Extend the sieve so that indices up to 'limit' (inclusive) are valid. */
static void ensure_sieve(size_t limit)
{
    if (limit < sieve_cap)
        return;                       /* already sufficient */

    /* Discard previous sieve if any. */
    if (sieve) {
        free(sieve);
        sieve = NULL;
        sieve_cap = 0;
    }

    /* Allocate new sieve. */
    sieve = (bool *)malloc((limit + 1) * sizeof(bool));
    if (sieve == NULL) {                /* allocation fail -> fall back */
        sieve_cap = 0;
        return;
    }

    /* Mark all as prime initially. */
    for (size_t i = 0; i <= limit; ++i)
        sieve[i] = true;
    sieve[0] = false;
    if (limit >= 1)
        sieve[1] = false;

    /* Eratosthenes sieve. */
    for (size_t p = 2; p * p <= limit; ++p) {
        if (!sieve[p])
            continue;
        for (size_t m = p * p; m <= limit; m += p)
            sieve[m] = false;
    }

    sieve_cap = limit + 1;
}

/* Check if x is prime, using the sieve if possible, otherwise trial division. */
static bool is_prime_fast(uint64_t x)
{
    if (x < sieve_cap && sieve != NULL)
        return sieve[(size_t)x];

    /* Infallible deterministic trial division for x <= 1e12 (our values <= ~1e6). */
    if (x < 2)
        return false;
    if (x % 2 == 0)
        return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0)
            return false;
    return true;
}

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Largest candidate for this n is 100*n + 97. */
    uint64_t max_candidate = ((uint64_t)n) * 100ULL + 97ULL;

    if (max_candidate >= sieve_cap)
        ensure_sieve((size_t)max_candidate);

    unsigned count = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t candidate = ((uint64_t)n) * 100ULL + (uint64_t)primes_lt100[i];
        if (is_prime_fast(candidate))
            ++count;
    }

    return (seq_t)count;
}
