/*
 * A182573  Primes that have six terms in their Zeckendorf representation.
 * https://oeis.org/A182573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A182573: primes that have six terms in their Zeckendorf representation.
   Formula: there is no simple closed form.  The function generates primes in
   increasing order, checks their Zeckendorf representation, and returns
   the nth prime with exactly 6 summands.  The algorithm does not depend on
   the size of seq_t; it uses a 64‑bit primality test and keeps the results
   in a dynamically resized array. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* 64‑bit deterministic Miller–Rabin primality test                    */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2)          return false;
    static const uint64_t small_primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof small_primes / sizeof small_primes[0]; ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }

    /* test bases; these are sufficient for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, power = a, exp = d;
        /* modular exponentiation: power^exp mod n */
        while (exp > 0) {
            if (exp & 1ULL) x = (x * power) % n;
            power = (power * power) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n-1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Fibonacci cache for Zeckendorf decomposition                       */
#define FIB_CACHE_SIZE 200
static seq_t fib_cache[FIB_CACHE_SIZE];
static size_t fib_len = 0;

static void ensure_fib(seq_t n)
{
    if (fib_len == 0) {
        fib_cache[0] = (seq_t)1;
        fib_cache[1] = (seq_t)2;
        fib_len = 2;
    }
    while (fib_cache[fib_len - 1] < n && fib_len < FIB_CACHE_SIZE) {
        fib_cache[fib_len] = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        ++fib_len;
    }
}

/* Count the number of terms in the Zeckendorf representation of n. */
static int zeckendorf_terms(seq_t n)
{
    if (n == 0) return 0;
    ensure_fib(n);
    int count = 0;
    while (n != (seq_t)0) {
        size_t k = fib_len - 1;
        /* largest fib <= n */
        while (k > 0 && fib_cache[k] > n) --k;
        n -= fib_cache[k];
        ++count;
    }
    return count;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
seq_t a(idx_t n)
{
    static seq_t *results = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static uint64_t next_candidate = 2;  /* next number to test for primality */

    if (n == 0) return 0;  /* undefined, but keep harmless */

    while (len < (size_t)n) {
        /* skip even numbers after 2 */
        if (next_candidate > 2 && (next_candidate & 1ULL) == 0) {
            ++next_candidate;
            continue;
        }

        if (is_prime_u64(next_candidate)) {
            if (zeckendorf_terms((seq_t)next_candidate) == 6) {
                if (len == cap) {
                    size_t new_cap = cap ? cap * 2 : 8;
                    seq_t *new_arr = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
                    results = new_arr;
                    cap = new_cap;
                }
                results[len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }
    return results[n - 1];
}
