/*
 * A106060  Primes p such that 1*p + 8 and 8*p + 1 are primes.
 * https://oeis.org/A106060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search primes p such that p+8 and 8p+1 are prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* 64‑bit modular multiplication using 128‑bit intermediate */

/* 64‑bit modular exponentiation */
static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mod_mul(r, a, m);
        a = mod_mul(a, a, m);
        e >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin test for all 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow(a % n, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ----------------------------------------------------- */

/* Storage for found primes of the sequence. */
static seq_t *primes = NULL;      /* array of returned values   */
static idx_t  prime_cnt = 0;      /* number of stored values    */
static idx_t  prime_cap = 0;      /* current capacity           */

/* Next odd integer to test for primality.  We start with 3. */
static uint64_t next_candidate = 3;

/* Ensure array can hold at least one more element. */
static void grow_array(void)
{
    idx_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
    seq_t *new_arr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    primes = new_arr;
    prime_cap = new_cap;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, but harmless */

    /* Generate terms until we have at least n of them. */
    while (prime_cnt < n) {
        /* Find the next prime p. */
        while (true) {
            if (next_candidate < 3) next_candidate = 3;
            if ((next_candidate & 1U) == 0) { next_candidate++; continue; }
            if (is_prime_uint64(next_candidate)) break;
            next_candidate += 2;          /* skip even numbers */
        }
        uint64_t p = next_candidate;

        /* Test the two extra conditions. */
        if (is_prime_uint64(p + 8) && is_prime_uint64(8 * p + 1)) {
            if (prime_cnt == prime_cap) grow_array();
            primes[prime_cnt++] = (seq_t)p;   /* store in requested type */
        }

        /* Advance to the next odd candidate. */
        next_candidate += 2;
    }

    return primes[n - 1];
}
