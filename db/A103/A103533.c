/*
 * A103533  Even semiprimes of the form prime(n)*prime(n+1) - 1.
 * https://oeis.org/A103533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 257, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 46 terms through n = 9962 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n): the n‑th term of OEIS A103533.
   a(n) is the n‑th even number of the form prime(i)*prime(i+1)-1
   for which (prime(i)*prime(i+1)-1)/2 is also prime.  The function
   generates consecutive primes incrementally and tests each candidate. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Deterministic Miller‑Rabin for 64‑bit integers */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,
                                            13ULL,17ULL,19ULL,23ULL,29ULL,
                                            31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Write n-1 = d * 2^s with d odd. */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* Bases sufficient for 64‑bit deterministic test. */
    const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
                              9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;            /* skip witnesses that equal 0 */
        __uint128_t x = 1, power = d;
        __uint128_t base = a;
        /* Compute a^d mod n using binary exponentiation. */
        while (power) {
            if (power & 1) x = (x * base) % n;
            base = (base * base) % n;
            power >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

static uint64_t *prime_table = NULL;   /* dynamic array of primes */
static size_t prime_count   = 0;       /* number of primes currently stored */

/* Ensure that prime_table contains at least 'needed' primes. */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed)
        return;

    uint64_t candidate = (prime_count == 0) ? 2 : prime_table[prime_count - 1] + 1;

    while (prime_count < needed)
    {
        bool ok = true;
        /* trial division using previously found primes */
        uint64_t limit = (uint64_t)sqrt((long double)candidate);
        for (size_t i = 0; i < prime_count && prime_table[i] <= limit; ++i)
        {
            if (candidate % prime_table[i] == 0)
            {
                ok = false;
                break;
            }
        }
        if (ok)
        {
            size_t new_cap = prime_count + 1;
            uint64_t *tmp = (uint64_t*)realloc(prime_table, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            prime_table = tmp;
            prime_table[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    size_t found = 0;
    size_t i = 0;            /* index into prime_table (prime_table[i]) */

    for (;;)
    {
        /* We need at least two primes for the pair i,i+1 */
        ensure_primes(i + 2);

        seq_t p1 = (seq_t)prime_table[i];
        seq_t p2 = (seq_t)prime_table[i + 1];
        seq_t val = p1 * p2 - (seq_t)1;

        /* Only even values are considered (odd value occurs only for (2,3)). */
        if ((val & (seq_t)1) == 0)
        {
            seq_t q = val / (seq_t)2;
            /* q fits into uint64_t for the ranges of interest */
            if (is_prime_uint64((uint64_t)q))
            {
                ++found;
                if (found == n)
                    return val;
            }
        }
        ++i;
    }
}
