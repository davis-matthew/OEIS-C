/*
 * A141972  Primes congruent to 17 mod 28.
 * https://oeis.org/A141972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime ≡ 17 (mod 28)
 * Range:     u8 n <= 5, u16 n <= 548, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 17, a(2) = 73, a(3) = 101, ... */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Simple deterministic Miller–Rabin test for 64‑bit integers.          */
/* This is fast, uses only integer arithmetic and works for all        */
/* 0 ≤ n < 2^64.  It is sufficient for all candidates used in this    */
/* sequence up to the limits required by the tests.                    */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n % small_primes[i] == 0) return n == small_primes[i];
    }

    /* write n−1 as d·2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Miller–Rabin witnesses sufficient for 64‑bit integers */
    static const uint64_t witnesses[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) return true;
        __uint128_t x = 1, p = a, d_local = d;
        /* compute a^d mod n using binary exponentiation */
        while (d_local) {
            if (d_local & 1) x = (x * p) % n;
            p = (p * p) % n;
            d_local >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (unsigned r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache of primes congruent to 17 (mod 28).                           */
/* ------------------------------------------------------------------ */
static seq_t *prime_cache = NULL;   /* dynamically resized array */
static size_t cache_size  = 0;       /* number of stored primes */
static size_t cache_cap   = 0;       /* allocated capacity */
static uint64_t next_m    = 0;       /* next multiplier to test */

/* Ensure that prime_cache contains the first n primes of the form
 * 28·k + 17.  The function grows the array as needed.                  */
static void ensure_cache(idx_t n)
{
    while (cache_size < (size_t)n) {
        uint64_t candidate = 17 + 28 * next_m++;
        if (is_prime_uint64(candidate)) {
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 32;
                prime_cache = (seq_t *)realloc(prime_cache,
                                                cache_cap * sizeof(seq_t));
            }
            prime_cache[cache_size++] = (seq_t)candidate;
        }
    }
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but harmless */
    ensure_cache(n);
    return prime_cache[n-1];
}
