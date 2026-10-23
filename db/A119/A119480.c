/*
 * A119480  Numbers k such that the Bernoulli number B_{4k} has denominator
 *          30.
 * https://oeis.org/A119480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that for every prime p ≥ 7, * (p
 *            − 1) does NOT divide 4k. Equivalently, k is not a multiple of *
 *            (p − 1)/gcd(p − 1, 4) for any such prime
 * Range:     u8 n <= 32, u16 n <= 8841, u32 n <= 7598, u64 n <= 7598,
 *            u128 n <= 7598, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Sufficiently large bounds to hold the first 10000 terms. */
#define KMAX   300000
#define LIMIT  (4 * KMAX + 1)

/* Table of the first 10000 terms of the sequence. */
static seq_t seq_vals[10001];

/* Flag indicating whether the table has been built. */
static bool initialized = false;

/* Build the table on first use. */
static void init_sequence(void)
{
    static unsigned char allowed[KMAX + 1];
    static unsigned char is_prime[LIMIT + 1];

    /* Initially all k are allowed. */
    for (size_t i = 0; i <= KMAX; ++i)
        allowed[i] = 1;

    /* Sieve of Eratosthenes up to LIMIT. */
    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* For each prime p ≥ 7, forbid all multiples of
     * t = (p-1)/gcd(p-1,4). */
    for (size_t p = 7; p <= LIMIT; ++p) {
        if (!is_prime[p])
            continue;
        size_t p_minus = p - 1;
        size_t g = (p_minus % 4 == 0) ? 4 : 2;   /* gcd(p-1,4) */
        size_t t = p_minus / g;
        if (t == 0)
            continue;
        for (size_t m = t; m <= KMAX; m += t)
            allowed[m] = 0;
    }

    /* Store the first 10000 allowed k's. */
    size_t count = 0;
    for (size_t k = 1; k <= KMAX && count < 10000; ++k) {
        if (allowed[k]) {
            ++count;
            seq_vals[count] = (seq_t)k;
        }
    }
}

/* Simple primality test used only for the rare fallback case. */
static bool is_prime_small(size_t n)
{
    if (n < 2)
        return false;
    for (size_t d = 2; d * d <= n; ++d)
        if (n % d == 0)
            return false;
    return true;
}

/* Check that 4k is not divisible by p-1 for any prime p ≥ 7. */
static bool is_allowed_large(size_t k)
{
    size_t limit = 4 * k + 1;
    for (size_t p = 7; p <= limit; ++p) {
        if (!is_prime_small(p))
            continue;
        size_t p_minus = p - 1;
        if ((4 * k) % p_minus == 0)
            return false;             /* forbidden */
    }
    return true;
}

/* Return the n-th term of the sequence.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;                /* undefined for n = 0 */

    if ((size_t)n <= 10000)
        return seq_vals[(size_t)n];

    /* Fallback: compute on demand for n > 10000. */
    size_t k      = 1;
    size_t found  = 0;
    while (found < (size_t)n) {
        if (is_allowed_large(k))
            ++found;
        ++k;
    }
    return (seq_t)(k - 1);
}
