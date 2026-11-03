/*
 * A228431  Number of ordered ways to write n = x + y (x, y > 0) with p(3, x)
 *          + p(6, y) prime, where p(3, k) denotes the triangular number
 *          k*(k+1)/2 and p(6, k) denotes the hexagonal number k*(2*k-1) =
 *          p(3, 2*k-1).
 * https://oeis.org/A228431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (x,y) with x>0, y>0, x+y=n such
 *            that p(3,x)+p(6,y) is prime, where p(3,k)=k*(k+1)/2 and
 *            p(6,k)=k*(2*k-1)
 * Range:     u8 n <= 12, u16 n <= 355, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 8465 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
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
    /* n = 1 gives no positive x,y with x+y=n */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    for (idx_t x = 1; x < n; ++x) {
        idx_t y = n - x;

        seq_t tri = (seq_t)x * ((seq_t)x + 1) / 2;
        seq_t hex = (seq_t)y * ((seq_t)2 * y - 1);
        seq_t sum = tri + hex;

        if (isPrimeGeneric(sum))
            ++count;
    }

    return count;
}
