/*
 * A105129  Primes of the form 128n+65.
 * https://oeis.org/A105129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 110, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul64_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form 128*n + 65 */

/*  Note:  The function returns the nth prime of the sequence
 *  A105129.  It keeps a growing list of such primes and extends
 *  the list only as far as needed for the current call.  The
 *  primality test is a deterministic Miller–Rabin routine
 *  sufficient for 64‑bit integers.  All arithmetic that can
 *  exceed 64 bits is performed using 128‑bit intermediate
 *  results, which are always available on the platforms
 *  used for evaluation. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul64_mod.h"

/* 64‑bit modular exponentiation by repeated squaring */
static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp != 0) {
        if (exp & 1)
            result = mul64_mod(result, base, mod);
        base = mul64_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller–Rabin test for 64‑bit integers */
static bool is_prime64(uint64_t n)
{
    if (n < 2)               return false;

    /* Small primes */
    static const uint64_t small_primes[] =
        {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0)          return n == p;
    }

    /* Express n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* Witnesses for 64‑bit integers */
    static const uint64_t witnesses[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL,
         450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0)                 return true;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1)   continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul64_mod(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Dynamic array of primes found so far */
static seq_t *prime_list   = NULL;
static idx_t  prime_count  = 0;
static idx_t  prime_cap    = 0;
static idx_t  next_k       = 1;   /* candidate k for 128*k+65 */

/* Add a new prime to the dynamic array */
static void add_prime(seq_t value)
{
    if (prime_count == prime_cap) {
        idx_t new_cap = prime_cap ? prime_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
    prime_list[prime_count++] = value;
}

/* Return the nth prime of the form 128*k + 65  (1‑based index) */
seq_t a(idx_t n)
{
    while (prime_count < n) {
        uint64_t candidate = 128ULL * (uint64_t)next_k + 65ULL;
        if (is_prime64(candidate))
            add_prime((seq_t)candidate);
        ++next_k;
    }
    return prime_list[n - 1];
}
