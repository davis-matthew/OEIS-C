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
 * Helpers:   is_palindrome, smallest_prime_factor
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
#include "../utils/is_palindrome.h"
#include "../utils/smallest_prime_factor.h"

/* Test whether a 64‑bit integer is a decimal palindrome. */
static bool is_pal_uint64(uint64_t x)
{
    return is_palindrome((seq_t)x, 10);
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
    return is_palindrome(sum, 10);
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
