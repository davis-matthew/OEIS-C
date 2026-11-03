/*
 * A157372  Number of ways to write the (n+50)-th positive odd integer in the
 *          form p+2^x+51*2^y with p an odd prime and x,y positive integers.
 * https://oeis.org/A157372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = number of triples (p,x,y) with p odd prime, x,y positive
 *            integers, and p + 2^x + 51·2^y = 2(n+50) – 1
 * Range:     u8 n <= 93, u16 n <= 199999, u32 n <= 200000, u64 n <= 200000,
 *            u128 n <= 200000, big any n
 * Validated: standard, pass, 76 terms through n = 9091 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, pow2
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
    /* offset: a(1) corresponds to n=1 */
    seq_t T = (seq_t)2 * (seq_t)n + 99;   /* 2(n+50) - 1 */
    seq_t count = 0;

    /* upper bound for 51·2^y: it must be <= T-5 (since p>=3, 2^x>=2) */
    seq_t max_y_term = (T > 5) ? (T - 5) : 0;
    seq_t mult = 51;

    for (idx_t y = 1; ; ++y) {
        seq_t pow2_y = pow2(y);                /* 2^y                                    */
        if (pow2_y == 0)                       /* overflow in pow2 implementation       */
            break;
        /* 51*2^y <= max_y_term  => 2^y <= max_y_term / 51            */
        if (pow2_y > max_y_term / mult)
            break;

        seq_t c1 = mult * pow2_y;              /* 51·2^y                                 */

        /* 2^x must satisfy 2^x + c1 <= T-3   (p >= 3) */
        seq_t max_x_term = (T > 3) ? (T - 3 - c1) : 0;

        for (idx_t x = 1; ; ++x) {
            seq_t pow2_x = pow2(x);            /* 2^x                                    */
            if (pow2_x == 0)                   /* overflow in pow2 implementation       */
                break;
            if (pow2_x > max_x_term)
                break;

            seq_t sum = pow2_x + c1;           /* p + 2^x + 51·2^y                        */
            seq_t p   = T - sum;               /* remaining part (must be prime)          */

            if (p > 2 && isPrimeGeneric(p))
                ++count;
        }
    }
    return count;
}
