/*
 * A130897  Numbers that are not exponentially squarefree.
 * https://oeis.org/A130897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2887, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   exponent_is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A130897 – numbers that are not exponentially squarefree.
   Each term is the smallest integer whose prime power factorization
   contains at least one exponent that is not squarefree.
   The implementation enumerates integers, tests each prime factor
   exponent for squarefreeness, and returns the n‑th such integer. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/exponent_is_squarefree.h"

/* ------------------------------------------------------------ */
/* Prime table – generated once on first use (to factor numbers). */
#define SIEVE_LIMIT 100000
static uint64_t primes[20000];
static size_t prime_cnt = 0;
static bool sieve_done = false;

static void init_primes(void)
{
    if (sieve_done) return;
    static bool is_composite[SIEVE_LIMIT + 1];
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) is_composite[i] = false;
    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_composite[j] = true;
    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_composite[i])
            primes[prime_cnt++] = i;
    sieve_done = true;
}

/* ------------------------------------------------------------ */

/* ------------------------------------------------------------ */
/* Determine whether val is a non‑e‑squarefree integer. */
static bool is_non_esquarefree(uint64_t val)
{
    uint64_t tmp = val;
    for (size_t i = 0; i < prime_cnt &&
                        (uint64_t)primes[i] * (uint64_t)primes[i] <= tmp;
         ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (!exponent_is_squarefree(exp))
                return true; /* found a non-squarefree exponent */
        }
    }
    /* Remaining factor > sqrt(val) has exponent 1 -> squarefree */
    return false;
}

/* ------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, return 0 as placeholder */
    init_primes();        /* ensure prime table is ready */

    idx_t count = 0;
    for (uint64_t candidate = 2;; ++candidate) {
        if (is_non_esquarefree(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
