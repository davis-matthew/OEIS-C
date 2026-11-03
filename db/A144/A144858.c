/*
 * A144858  Numbers of the form 2^(n-1)*(2^n - 1) which aren't perfect
 *          numbers.
 * https://oeis.org/A144858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0; for n≥2, a(n) = 2^(k-1)*(2^k-1) where k is the (n‑1)-th
 *            positive integer such that 2^k‑1 is NOT prime (i.e., k is not
 *            the exponent of a Mersenne prime)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/pow2.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* a(1) = 0 by definition */

    idx_t target = n - 1;        /* we look for the (n‑1)-th valid k */
    idx_t count  = 0;

    for (idx_t k = 1; ; ++k)
    {
        bool skip = false;

        /* Only need to test if k is prime; otherwise 2^k‑1 cannot be prime. */
        if (k > 1 && is_prime(k))
        {
            seq_t m = pow2(k);
            m -= 1;            /* m = 2^k - 1 */

            /* Mersenne prime? If yes, the corresponding number is perfect. */
            if (isPrimeGeneric(m))
                skip = true;
        }

        if (!skip)                 /* k gives a non‑perfect number */
        {
            ++count;
            if (count == target)
            {
                seq_t two_k_minus1 = pow2(k - 1);
                seq_t two_k       = pow2(k);
                return two_k_minus1 * (two_k - 1);
            }
        }
    }
}
