/*
 * A107195  Primes of the form 7x^2 + 12y^2.
 * https://oeis.org/A107195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 411, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_of_form
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107195 – primes of the form 7*x^2 + 12*y^2 (OEIS).
   The function enumerates successive primes and keeps those that can be
   expressed as 7*x^2 + 12*y^2.  It is intentionally order‑independent
   and memoises already found terms. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_of_form.h"

/* --------------------------------------------------------------------- */
/*  Deterministic Miller–Rabin primality test for 64‑bit unsigned integers
   --------------------------------------------------------------------- */

static inline uint64_t modpow(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t result = 1;
    while (d) {
        if (d & 1)
            result = (unsigned __int128)result * a % mod;
        a = (unsigned __int128)a * a % mod;
        d >>= 1;
    }
    return result;
}

static bool isPrime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                    19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] =
        {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (cont) continue;
        return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Check whether a prime can be written as 7*x^2 + 12*y^2
   --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Main sequence function
   --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; avoid UB */

    /* static memoisation data */
    static seq_t *seq = NULL;
    static idx_t count = 0;
    static idx_t capacity = 0;
    static uint64_t candidate = 2;   /* next number to test */

    while (count < n) {
        uint64_t p = candidate;
        /* update candidate for next iteration */
        if (candidate <= 2)
            candidate = 3;
        else
            candidate += 2;   /* skip even numbers after 2 */

        if (isPrime_uint64(p) && is_of_form(p)) {
            if (count == capacity) {
                capacity = capacity ? capacity * 2 : 8;
                seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
            }
            seq[count++] = (seq_t)p;
        }
    }

    return seq[n - 1];
}
