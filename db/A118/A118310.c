/*
 * A118310  a(n) = gcd(n,m(n)), where m(n) is the n-th nonprime positive
 *          integer (1 or composite).
 * https://oeis.org/A118310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(n, m(n)), where m(n) is the n‑th nonprime (1 or
 *            composite) integer
 * Range:     u8 n <= 2546, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 119 terms through n = 9803 (2026-11-03)
 * Helpers:   gcd, is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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
    if (n == 0)   /* the sequence is 1‑based; value for n==0 is undefined */
        return 0;

    idx_t candidate = 1;
    idx_t count = 0;

    /* search for the n‑th nonprime integer */
    while (true) {
        if (!isPrimeGeneric((seq_t)candidate)) {
            ++count;
            if (count == n) break;
        }
        ++candidate;
    }

    /* compute gcd of n and the found nonprime */
    return gcd((seq_t)n, (seq_t)candidate);
}
