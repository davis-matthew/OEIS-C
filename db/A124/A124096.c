/*
 * A124096  Primes p that divide Fibonacci[(p+1)/3].
 * https://oeis.org/A124096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 549, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th prime p such that  p divides Fibonacci[(p+1)/3] */
/* This implementation keeps a list of all qualified primes found so far.  It uses
 * the fast‑doubling Fibonacci algorithm modulo p, and the get_prime helper
 * from the utils package to obtain successive primes.  The list is stored as
 * 64‑bit unsigned integers (the usual size of the found primes).  When
 * a value is returned it is cast to seq_t, which works for both normal
 * integral sizes and for the arbitrary‑precision build.  The algorithm
 * runs in O(n log n) time and keeps the memory footprint well below
 * the 2‑second limit for n up to 10 000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/get_prime.h"

/* Fast doubling Fibonacci modulo a modulus fitting into 64 bits. */
static uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    if (n == 0) return 0;
    uint64_t a = 0;          /* F(0) */
    uint64_t b = 1;          /* F(1) */
    int msb = 63 - __builtin_clzll(n); /* highest set bit */

    for (int i = msb; i >= 0; --i) {
        /* Compute F(2k) and F(2k+1) from F(k)=a and F(k+1)=b */
        uint64_t t = ((__uint128_t)b * 2ULL) % mod;
        uint64_t c = ((__uint128_t)a * ((t + mod - a) % mod)) % mod;   /* F(2k) */
        uint64_t d = (( (__uint128_t)a * a ) % mod + ( (__uint128_t)b * b ) % mod) % mod; /* F(2k+1) */

        if ((n >> i) & 1ULL) {
            /* use F(2k+1), F(2k+2) = F(2k)+F(2k+1) */
            a = d;
            b = (c + d) % mod;
        } else {
            /* use F(2k), F(2k+1) */
            a = c;
            b = d;
        }
    }
    return a;  /* F(n) mod mod */
}

/* Static state for memoization */
static idx_t     next_prime_index = 0;   /* index used with get_prime()   */
static size_t    good_cnt         = 0;   /* number of qualified primes    */
static size_t    good_cap         = 0;   /* allocation capacity           */
static uint64_t *good_primes_u64  = NULL;/* primes that satisfy the test  */

/* Ensure at least 'n' qualified primes have been found and stored. */
static void ensure_terms(idx_t n)
{
    if (n == 0) return;
    while (good_cnt < n) {
        /* Fetch next prime.  get_prime returns uint32_t, convert to 64-bit. */
        uint32_t pk = get_prime(next_prime_index);
        next_prime_index++;
        uint64_t p = (uint64_t)pk;

        /* Candidate must be > 3 and satisfy p ≡ 2 (mod 3). */
        if (p <= 3 || (p % 3) != 2) continue;

        uint64_t k = (p + 1) / 3;
        if (fib_mod_uint64(k, p) == 0) {
            /* Store the qualified prime.  Resize storage if needed. */
            if (good_cnt == good_cap) {
                size_t new_cap = good_cap == 0 ? 8 : good_cap * 2;
                uint64_t *tmp = (uint64_t*)malloc(sizeof(uint64_t) * new_cap);
                if (!tmp) {
                    /* Allocation failure – abort the program */
                    abort();
                }
                if (good_primes_u64) {
                    memcpy(tmp, good_primes_u64, sizeof(uint64_t) * good_cnt);
                    free(good_primes_u64);
                }
                good_primes_u64 = tmp;
                good_cap = new_cap;
            }
            good_primes_u64[good_cnt++] = p;
        }
    }
}

/* Main function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for 0 – return 0 */
    ensure_terms(n);
    return (seq_t)good_primes_u64[n - 1];
}
