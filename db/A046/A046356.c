/*
 * A046356  Odd numbers with only palindromic prime factors whose sum is
 *          palindromic (counted with multiplicity).
 * https://oeis.org/A046356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 41 (2026-11-03)
 * Helpers:   gcd_u64, is_palindrome_base, is_prime_u64, mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A046356: odd numbers whose prime factors are all
 * palindromic primes and whose sum of prime factors (with multiplicity)
 * is also a palindrome.  This implementation uses a brute‑force search
 * and the helper function smallest_prime_factor to factor each
 * candidate.  The function returns the n-th term with a(0) = 9.  */

/* Utility includes */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/is_palindrome_base.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"

/* Pollard–Rho: find a non‑trivial divisor of n (n composite) */
static uint64_t pollard(uint64_t n) {
    if (n % 2 == 0) return 2;
    uint64_t c = 1;
    while (1) {
        uint64_t x = 2, y = 2, d = 1;
        while (d == 1) {
            x = (mulmod(x, x, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            y = (mulmod(y, y, n) + c) % n;
            uint64_t diff = x > y ? x - y : y - x;
            d = gcd_u64(diff, n);
        }
        if (d != n) return d;
        ++c;  /* try another constant */
    }
}

/* recursive smallest prime factor */
static uint64_t smallest_prime_factor(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (is_prime_u64(n)) return n;
    uint64_t d = pollard(n);
    uint64_t s1 = smallest_prime_factor(d);
    uint64_t s2 = smallest_prime_factor(n / d);
    return (s1 < s2) ? s1 : s2;
}

/* Test whether a 64‑bit integer is a decimal palindrome. */
static bool is_pal_uint64(uint64_t x)
{
    return is_palindrome_base((seq_t)x, 10);
}

/* Checks whether the given odd composite number satisfies the
 * sequence conditions.  Returns true only if all prime factors are
 * palindromic and their sum is also a palindrome. */
static bool candidate_ok(uint64_t cand)
{
    if (cand <= 1 || (cand & 1ULL) == 0) return false; /* only odd >1 */

    uint64_t n = cand;
    seq_t sum   = 0;
    unsigned factor_count = 0;

    while (n != 1)
    {
        uint64_t p = smallest_prime_factor(n);
        if (!is_pal_uint64(p))
            return false;            /* non‑palindromic prime factor */

        /* Add the prime factor once per occurrence. */
        do {
            sum += (seq_t)p;
            ++factor_count;
            n /= p;
        } while (n % p == 0);
    }

    /* Must be composite. */
    if (factor_count <= 1) return false;

    /* Sum of prime factors must also be a palindrome. */
    return is_palindrome_base(sum, 10);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t found     = 0;
    uint64_t cand   = 9; /* first odd composite number */

    for (;;)
    {
        if (candidate_ok(cand))
        {
            if (found == n)
                return (seq_t)cand;
            ++found;
        }

        cand += 2; /* only odd numbers */
    }
}
