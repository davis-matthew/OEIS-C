/*
 * A080363  Nonprime numbers k such that the largest prime divisor of k is
 *          unitary.
 * https://oeis.org/A080363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nonprime numbers k such that the largest prime divisor of
 *            k is unitary
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9394 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (i.e. k is composite and its largest prime factor appears with exponent 1) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return true iff v is composite and its largest prime factor occurs
   with exponent exactly one.  Implements trial division because
   v is small (up to a few hundred thousand). */
static bool
is_largest_prime_unary(uint64_t v)
{
    if (v < 6)    /* primes <6 and 4 are excluded */
        return false;

    uint64_t n = v;
    uint64_t largest = 0;
    int largest_exp = 0;

    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int exp = 0;
            do {
                n /= p;
                ++exp;
            } while (n % p == 0);
            if (p > largest) {
                largest = p;
                largest_exp = exp;
            } else if (p == largest) {
                largest_exp += exp;
            }
        }
    }

    if (n > 1) {   /* remaining prime factor */
        if (n > largest) {
            largest = n;
            largest_exp = 1;
        } else if (n == largest) {
            largest_exp += 1;
        }
    }

    /* v is composite iff largest != v */
    if (largest == v)
        return false;

    return largest_exp == 1;
}

/* Maximum number of terms we cache (covers n up to about 10000) */
#define CACHE_CAP 10000

/* Cached sequence values */
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;      /* number of terms already computed */
static uint64_t next_candidate = 2;   /* next integer to test */

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        uint64_t v = next_candidate++;
        if (is_largest_prime_unary(v)) {
            cache[cache_len++] = (seq_t)v;
        }
    }
    return cache[n - 1];
}
