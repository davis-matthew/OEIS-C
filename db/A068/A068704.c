/*
 * A068704  a(n) = smallest prime obtained as the concatenation of n^k,
 *          n^(k-1), n^(k-2), ..., n^2, n, 1 for some k >= 1; or 0 if no such
 *          prime exists.
 * https://oeis.org/A068704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, hard, more, nonn
 * Formula:   a(n) = smallest prime obtained by concatenating n^k , n^(k-1) ,
 *            … , n , 1 for some k ≥ 1; 0 if none
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

seq_t a(idx_t n) {
    if (n < 1) return 0;                 /* not defined for n < 1 */

    seq_t n_seq = (seq_t)n;
    const size_t MAX_K = 200;            /* upper bound for search */

    /* pre-allocate powers up to MAX_K */
    seq_t powers[MAX_K + 1];
    powers[0] = (seq_t)1;
    size_t current_k = 0;

    for (idx_t k = 1; k <= (idx_t)MAX_K; ++k) {
        /* extend the powers array if necessary */
        for (size_t i = current_k + 1; i <= (size_t)k; ++i) {
            powers[i] = powers[i - 1] * n_seq;
        }
        current_k = (size_t)k;

        /* build the concatenation n^k , … , 1 */
        seq_t val = (seq_t)0;
        for (int exp = k; exp >= 0; --exp) {
            seq_t part = powers[exp];
            int d = num_digits(part);
            seq_t powd = pow10((idx_t)d);
            val = val * powd + part;
        }

        /* test for primality */
        if (isPrimeGeneric(val)) {
            return val;   /* first (hence smallest) prime found */
        }
    }

    /* no prime found within the search limit */
    return 0;
}
