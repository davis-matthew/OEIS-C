/*
 * A062669  Every divisor (except 1) contains the digit 4.
 * https://oeis.org/A062669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 2489, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_digit4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Every divisor (except 1) contains the digit 4 (OEIS A062669) */
/* This implementation searches for the numbers satisfying the
   property.  It keeps a static cache big enough for the expected
   range (n up to about 10000).  The algorithm is O(n * sqrt(a(n))) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/has_digit4.h"

/* Recursively verifies that every divisor of the number represented by
   current (a partial product of prime powers) contains the digit 4.
   primes[] and exps[] hold the prime factorisation of the original
   candidate.  The recursion explores all divisor combinations. */
static bool check_divisors(int idx, const uint64_t primes[], const int exps[],
                           int pcnt, seq_t current)
{
    if (idx == pcnt) {
        /* current is a divisor of the original candidate.  It is
           either 1 (which is allowed) or must contain the digit 4. */
        return (current == 1) || has_digit4(current);
    }
    uint64_t p = primes[idx];
    int emax = exps[idx];
    seq_t val = current;
    for (int e = 0; e <= emax; ++e) {
        if (!check_divisors(idx + 1, primes, exps, pcnt, val))
            return false;
        val = val * p;   /* multiply by prime for next exponent */
    }
    return true;
}

/* Tests whether all divisors > 1 of n contain the digit 4. */
static bool allDivHasFour(idx_t n)
{
    idx_t temp = n;
    uint64_t primes[64];
    int exps[64];
    int pcnt = 0;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            primes[pcnt] = p;
            exps[pcnt] = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exps[pcnt];
            }
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt] = 1;
        ++pcnt;
    }

    return check_divisors(0, primes, exps, pcnt, (seq_t)1);
}

/* Cache big enough for typical test ranges.  For larger n the cache
   can be resized by increasing CACHE_SIZE. */
#define CACHE_SIZE 20000

seq_t a(idx_t n)
{
    static seq_t cache[CACHE_SIZE];
    static idx_t cache_len = 0;
    static idx_t last_checked = 1;   /* start searching after 1 */

    /* Generate values until the cache contains at least n elements. */
    while (cache_len < n) {
        /* Find the next candidate that satisfies the condition. */
        for (;;) {
            ++last_checked;
            /* The number itself must contain the digit 4. */
            if (!has_digit4(last_checked))
                continue;
            if (allDivHasFour(last_checked)) {
                /* Store it in the cache. */
                if (cache_len >= CACHE_SIZE) {
                    /* In practice this should not happen for the intended
                       limits (n <= about 10000). */
                    abort();
                }
                cache[cache_len++] = (seq_t)last_checked;
                break;
            }
        }
    }
    return cache[n - 1];   /* a(1) is the first term */
}
