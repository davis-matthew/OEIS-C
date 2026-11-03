/*
 * A212167  Numbers k such that the maximum exponent in its prime
 *          factorization is not greater than the number of positive exponents
 *          (A051903(k) <= A001221(k)).
 * https://oeis.org/A212167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that the maximum exponent in its prime
 *            factorization is <= the number of distinct prime factors
 * Range:     u8 n <= 210, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9721 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Helper function: check condition for a 64‑bit integer.
   Returns true if k satisfies the criterion. */
static bool a212167_valid_uint64(uint64_t k)
{
    if (k == 1)            /* 1 is considered valid */
        return true;

    uint64_t distinct = 0;     /* number of distinct prime factors (ω) */
    uint64_t max_exp  = 0;     /* maximum exponent (emax) */
    uint64_t n = k;

    /* factor out powers of 2 first */
    if ((n & 1ULL) == 0) {
        uint64_t e = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            ++e;
        }
        ++distinct;
        if (e > max_exp) max_exp = e;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            ++distinct;
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            if (e > max_exp) max_exp = e;
        }
    }

    /* if n > 1, it is a remaining prime factor */
    if (n > 1) {
        ++distinct;
        if (1 > max_exp) max_exp = 1;
    }

    return max_exp <= distinct;
}

/* Main sequence function (external linkage). */
seq_t a(idx_t n)
{
    /* n is 1‑based offset. We simply search natural numbers until
       we have found n valid numbers. */
    uint64_t found = 0;
    uint64_t candidate = 1;

    while (found < n) {
        if (a212167_valid_uint64(candidate))
            ++found;
        ++candidate;
    }

    /* candidate has been incremented one past the desired number */
    return (seq_t)(candidate - 1);
}
