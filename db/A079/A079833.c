/*
 * A079833  Define f(n) = n + tau(n), then a(n) = smallest prime of the type
 *          f(f(f...f(n)))).
 * https://oeis.org/A079833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the first prime that appears from iterating f(n) = n +
 *            τ(n) (apply f at least once)
 * Range:     u8 n <= 39, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
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

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Compute τ(n) for an arbitrary seq_t value */
static seq_t tau_seq(seq_t n)
{
    seq_t t = n;
    seq_t result = 1;
    seq_t limit = isqrt_seq(t);           /* floor(√t) */
    for (seq_t p = 2; p <= limit; ++p) {
        if (t % p == 0) {
            seq_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            result *= (e + 1);
            /* t decreased ⇒ recompute limit to shorten loop */
            limit = isqrt_seq(t);
        }
    }
    if (t > 1)          /* final prime factor > √original t */
        result *= 2;
    return result;
}

/* Sequence a(n) from OEIS A079833 */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;         /* start value */
    do {
        cur += tau_seq(cur);      /* apply f once */
    } while (!isPrimeGeneric(cur));
    return cur;
}
