/*
 * A142625  Primes congruent to 34 mod 55.
 * https://oeis.org/A142625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 167, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 958 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A142625:
   a(n) = the nth prime p such that p ≡ 34 (mod 55)   (offset 1)
   The implementation generates primes of the form 55*k+34
   on demand, storing them in a static array to support
   calls in any order.  Each candidate is tested with the
   generic primality routine is_prime_big. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static uint64_t *primes = NULL;   /* primes ≡ 34 (mod 55) */
    static size_t prime_count = 0;    /* number of primes stored */
    static size_t prime_cap   = 0;    /* array capacity */
    static idx_t   next_k      = 0;    /* next k to try in 55*k+34 */

    /* Grow the array when needed (non‑class types). */
    if (prime_count == prime_cap)
    {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) return (seq_t)0;                 /* allocation failed */
        primes     = tmp;
        prime_cap  = new_cap;
    }

    /* Generate primes until we have at least n of them. */
    while (prime_count < (size_t)n)
    {
        uint64_t candidate = 55ULL * next_k + 34ULL;
        if (is_prime_big((seq_t)candidate))
        {
            primes[prime_count++] = candidate;
        }
        ++next_k;
    }

    /* Return the requested prime (offset 1). */
    return (seq_t)primes[n - 1];
}
