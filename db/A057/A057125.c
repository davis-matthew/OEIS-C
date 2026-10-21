/*
 * A057125  Numbers n such that 3 is a square mod n.
 * https://oeis.org/A057125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A057125: numbers n such that 3 is a square modulo n.
   Implementation uses incremental search and prime‑factor checks:
   - skip n if divisible by 4 or 9;
   - for every odd prime factor p > 3, require p ≡ ±1 (mod 12);
   - primes 2 and 3 are allowed at most once (checked by the divisibility test).
   The algorithm generates primes up to a fixed limit via a sieve and then
   tests each integer in turn until the requested index is reached.
   This method runs in sub‑second time for n up to about 1 000 000. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Prime sieve limit – more than enough for the test range. */
#define PRIME_LIMIT 100000

/* Static storage for the primes. */
static uint64_t primes[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_init = false;

/* Initialise the prime list once. */
static void init_primes(void)
{
    if (primes_init) return;
    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if (i * i <= PRIME_LIMIT) {
                for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    primes_init = true;
}

/* Test whether a given unsigned 64‑bit integer satisfies the
   conditions for being a term of the sequence. */
static bool passes_candidate(uint64_t m)
{
    /* Eliminate multiples of 4 or 9 (exponents of 2 or 3 too high). */
    if ((m % 4) == 0 || (m % 9) == 0) return false;

    uint64_t x = m;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if ((__int128)p * p > x) break;
        if (x % p == 0) {
            if (p == 2 || p == 3) {
                /* Remove all copies of 2 or 3 – they must be at most one. */
                while (x % p == 0) x /= p;
                continue;
            }
            /* For odd primes > 3 require p ≡ ±1 (mod 12). */
            if (p % 12 != 1 && p % 12 != 11) return false;
            while (x % p == 0) x /= p;
        }
    }
    /* Remaining factor, if any, is a prime > sqrt(m). */
    if (x > 1) {
        if (x == 2 || x == 3) return true;
        if (x % 12 != 1 && x % 12 != 11) return false;
    }
    return true;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    init_primes();

    idx_t count = 0;
    uint64_t cand = 1;
    while (true) {
        if (passes_candidate(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}
