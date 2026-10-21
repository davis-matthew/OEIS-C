/*
 * A077153  Smallest k such that there are n composite numbers greater than n
 *          and smaller than k.
 * https://oeis.org/A077153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A077153:  Smallest k such that there are n composite integers
   greater than n and smaller than k.  The function searches
   incrementally for the required number of composites. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Deterministic Miller–Rabin test for 64‑bit integers.  */
static bool is_prime64(uint64_t n)
{
    if (n < 2)            return false;
    if (n % 2 == 0)       return n == 2;
    if (n % 3 == 0)       return n == 3;

    const uint64_t small_primes[] = {
        5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL
    };
    const int small_cnt = sizeof small_primes / sizeof small_primes[0];
    for (int i = 0; i < small_cnt; ++i)
        if (n == small_primes[i]) return true;
    if (n % 5 == 0) return false;   /* n divisible by 5 but not 5 itself */

    /* Write n‑1 as d * 2^r with d odd. */
    uint64_t d = n - 1;
    unsigned r = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++r;
    }

    const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    const int base_cnt = sizeof bases / sizeof bases[0];

    for (int ib = 0; ib < base_cnt; ++ib) {
        uint64_t a = bases[ib] % n;
        if (a == 0) return true;

        /* Compute a^d mod n. */
        __uint128_t x = 1;
        __uint128_t p = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1) x = (x * p) % n;
            p = (p * p) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned j = 1; j < r; ++j) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Count composite integers strictly greater than n until
       exactly n of them have been seen. */
    uint64_t target = (uint64_t)n;          /* number of composites required */
    uint64_t current = target + 1;          /* first integer to examine */
    uint64_t count   = 0;                   /* composites seen so far */

    while (count < target) {
        if (current > 1 && !is_prime64(current)) { /* composite */
            ++count;
        }
        ++current;
    }

    /* After exiting, 'current' is the minimal k with exactly 'target'
       composites in (n, k).  Return it. */
    return (seq_t)current;
}
