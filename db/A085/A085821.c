/*
 * A085821  Smallest prime > A085820(n) ending with A085820(n).
 * https://oeis.org/A085821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  full, nonn, fini, base
 * Formula:   a(n) = smallest prime > A085820(n) ending with A085820(n), where
 *            A085820(n) is the nth odd number not divisible by 5
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: full, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"

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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Compute the nth odd number not divisible by 5
       A085820(n) = 10 * floor((n-1)/4) + {1,3,7,9}[ (n-1) mod 4 ]   */
    static const seq_t offs[4] = {1, 3, 7, 9};
    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;
    seq_t t = (seq_t)(10 * q + offs[r]);

    /* Number of decimal digits of t */
    int d = num_digits(t);

    /* 10^d */
    seq_t m = pow10((idx_t)d);

    for (idx_t k = 1; ; ++k) {
        seq_t candidate = ((seq_t)k * m) + t;  /* candidate > t */
        if (candidate >= 100 && isPrimeGeneric(candidate))
            return candidate;
    }
}
