/*
 * A091367  Primes p such that the sum of the digits raised to the 4th power
 *          is prime.
 * https://oeis.org/A091367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 568, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A091367  : The nth prime p such that Σ(d^4) over its decimal digits is prime
 *  Formula: a(n) = the nth prime p ∈ ℙ for which Σ_{digits d of p} d⁴ is prime
 *
 *  The function uses a simple iterative prime search with deterministic
 *  Miller‑Rabin primality testing.  The digit‑sum of fourth powers is
 *  computed with a small table of 0², … ,9⁴, and the sum is tested for
 *  primality with the same Miller‑Rabin routine.
 *
 *  A dynamic cache holds already found terms so that calls in any order
 *  do not recompute earlier terms.  The implementation is fully
 *  compatible with C11 and C++17 (no C++ only constructs are used).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* Utility: deterministic Miller-Rabin for 64-bit integers */

static bool is_prime_u64(uint64_t n)
{
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    static const uint64_t bases[] = {2,3,5,7,11,13,17};

    if (n < 2) return false;
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); i++) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); i++) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        unsigned __int128 x = 1;
        unsigned __int128 base = a % n;
        uint64_t exp = d;
        /* binary exponentiation */
        while (exp) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  sum of fourth powers of decimal digits */

static const uint64_t pow4[10] = {
    0ULL, 1ULL, 16ULL, 81ULL, 256ULL, 625ULL,
    1296ULL, 2401ULL, 4096ULL, 6561ULL
};

static uint64_t digit_fourth_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x) {
        sum += pow4[x % 10];
        x /= 10;
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/*  Cache of computed terms */

static seq_t *cache_vals = NULL;      /* array of seq_t */
static size_t cache_len   = 0;        /* number of terms stored */
static size_t cache_cap   = 0;        /* allocated capacity */
static uint64_t next_candidate = 2ULL;  /* next number to test for primality */

/* ensure capacity of cache at least 'min_cap' */
static void ensure_cache_cap(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_arr = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    if (!new_arr) abort();  /* allocation failure */
    cache_vals = new_arr;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 0 */

    /* Build cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        uint64_t candidate = next_candidate;
        /* Find next prime */
        while (!is_prime_u64(candidate)) {
            candidate++;
        }
        /* candidate is prime */
        uint64_t sum = digit_fourth_sum(candidate);
        if (is_prime_u64(sum)) {
            /* store this prime into cache */
            ensure_cache_cap(cache_len + 1);
            cache_vals[cache_len++] = (seq_t)candidate;
        }
        next_candidate = candidate + 1;
    }

    return cache_vals[n - 1];
}
