/*
 * A078779  Union of S, 2S and 4S, where S = odd squarefree numbers (A056911).
 * https://oeis.org/A078779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 7098, u32 n <= 7098, u64 n <= 7098,
 *            u128 n <= 7098, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A078779 – Union of odd squarefree numbers, twice them, and four times them.
   Formula: a(n) is the n‑th positive integer that has exponent of 2 ≤ 2
   and is odd‑squarefree (all odd prime exponents are 0 or 1). */
/* The implementation scans integers in increasing order,
   tests the described condition and stops after n valid integers have been found. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Static helper: generate odd primes up to 65536 (safe for all needed ops) */
static uint32_t *odd_primes = NULL;
static size_t odd_prime_cnt = 0;

static void init_odd_primes(void)
{
    if (odd_primes != NULL)
        return;                      /* already built */

    const size_t limit = 65536;        /* upper bound for sieve */
    /* simple sieve of Eratosthenes */
    bool *is_prime = (bool*)malloc(limit + 1);
    if (!is_prime) return;            /* allocation failure – will make the function crash later */

    for (size_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (size_t q = p * p; q <= limit; q += p)
                is_prime[q] = false;

    /* count odd primes */
    odd_prime_cnt = 0;
    for (size_t p = 3; p <= limit; p += 2)
        if (is_prime[p]) ++odd_prime_cnt;

    odd_primes = (uint32_t*)malloc(odd_prime_cnt * sizeof(uint32_t));
    if (!odd_primes) { free(is_prime); return; }

    /* store odd primes */
    size_t idx = 0;
    for (size_t p = 3; p <= limit; p += 2)
        if (is_prime[p])
            odd_primes[idx++] = (uint32_t)p;

    free(is_prime);
}

/* --------------------------------------------------------------------- */
/* Test if an odd number m is squarefree (i.e., no odd prime square divides m) */
static bool odd_squarefree(seq_t m)
{
    init_odd_primes();

    for (size_t i = 0; i < odd_prime_cnt; ++i) {
        seq_t p = (seq_t)odd_primes[i];
        seq_t sq = p * p;            /* p^2 fits in seq_t (≤ 2^32-1) */
        if (sq > m) break;
        if (m % sq == 0)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Main function as required by the test harness */
seq_t a(idx_t n)
{
    idx_t count = 0;      /* how many valid numbers found so far */
    idx_t candidate = 1;  /* current integer being tested */

    while (1) {
        idx_t temp = candidate;
        unsigned e2 = 0;

        /* Count exponent of 2, but stop if it exceeds 2 */
        while ((temp & 1) == 0) {
            ++e2;
            temp >>= 1;
            if (e2 > 2) break;
        }
        if (e2 <= 2) {
            /* temp is now the odd part of candidate */
            if (odd_squarefree((seq_t)temp)) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;   /* found the n-th term */
            }
        }
        ++candidate;
    }
}
