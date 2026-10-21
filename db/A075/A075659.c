/*
 * A075659  Sum of prime divisors (A008472) is a power of an integer with
 *          exponent greater than 1.
 * https://oeis.org/A075659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075659: numbers n such that the sum of distinct prime divisors of n
   (A008472(n)) is a perfect power with exponent > 1. 
   The routine builds a table of smallest prime factors on demand
   and scans integers until the requested number of terms has been found. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>

/* Global state for the on‑demand sieve */
static idx_t *spf = NULL;          /* smallest prime factor for each index */
static idx_t spf_limit = 0;        /* current size of spf[] */
static idx_t last_checked = 1;     /* last n that has been examined */
static idx_t found_count = 0;      /* how many valid terms have been found */

/* Allocate a new sieve of length "limit" (inclusive).  The array contains
   the smallest prime factor for every integer from 2 up to 'limit'.
   All entries for indices 0 and 1 are left zero. */
static void build_spf(idx_t limit)
{
    free(spf);
    spf = (idx_t *)calloc((size_t)limit + 1, sizeof(idx_t));
    if (!spf) return;   /* silently ignore; the caller will fail gracefully */

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;      /* i is prime */
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* Ensure the sieve covers at least 'n'.  If not, grow it by doubling
   the current limit or to 'n', whichever is larger. */
static void ensure_spf(idx_t n)
{
    if (n <= spf_limit) return;
    idx_t new_limit = spf_limit ? spf_limit * 2 : n * 2;
    if (new_limit < n) new_limit = n;
    if (new_limit > 0 && new_limit <= SIZE_MAX)
        build_spf(new_limit);
}

/* Compute the sum of distinct prime divisors of 'n' using the spf table. */
static seq_t sum_distinct_prime_divisors(idx_t n)
{
    seq_t sum = 0;
    while (n > 1) {
        idx_t p = spf[n];
        sum += (seq_t)p;
        while (n % p == 0) n /= p;
    }
    return sum;
}

/* Return true if 'x' is a perfect power with exponent >= 2.  Uses
   unsigned __int128 (available in both C and C++ compilers that
   support the code). */
static bool is_perfect_power(seq_t x)
{
    if (x < 4) return false;   /* 2^2 = 4 is the smallest perfect power */

    unsigned __int128 v = (unsigned __int128)x;
    for (unsigned long long base = 2; (unsigned __int128)base * base <= v; ++base) {
        unsigned __int128 p = (unsigned __int128)base * base;
        while (p <= v) {
            if (p == v) return true;
            p *= base;
        }
    }
    return false;
}

/* Main function requested by the tests.  Finds the nth term of the
   sequence. */
seq_t a(idx_t n)
{
    while (found_count < n) {
        ++last_checked;
        ensure_spf(last_checked);
        seq_t sum = sum_distinct_prime_divisors(last_checked);
        if (is_perfect_power(sum)) {
            ++found_count;
            if (found_count == n)
                return (seq_t)last_checked;
        }
    }
    return 0;   /* should never be reached */
}
