/*
 * A197596  a(n) = Floor((7n+1/n)^n).
 * https://oeis.org/A197596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor((7*n + 1/n)^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* = floor((7*n^2 + 1)^n / n^n) */
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"

static inline _Bool mul_uint128_overflow(unsigned __int128 a,
                                         unsigned __int128 b,
                                         unsigned __int128 *res)
{
    if (a == 0 || b == 0) {
        *res = 0;
        return 0;
    }
    unsigned __int128 max = ~((unsigned __int128)0);
    if (a > max / b) {        /* multiplication would overflow 128 bits */
        return 1;
    }
    *res = a * b;
    return 0;
}

/* exponentiation by squaring with overflow detection */
static _Bool pow_uint128_overflow(unsigned __int128 base,
                                  idx_t exp,
                                  unsigned __int128 *result)
{
    unsigned __int128 res = 1;
    unsigned __int128 cur = base;
    while (exp) {
        if (exp & 1u) {
            if (mul_uint128_overflow(res, cur, &res))
                return 1;
        }
        exp >>= 1u;
        if (exp) {
            if (mul_uint128_overflow(cur, cur, &cur))
                return 1;
        }
    }
    *result = res;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is defined for n>=1 */
        return 0;

    /* For small seq_t types (<=64 bits) use 128‑bit arithmetic to avoid
       overflow when computing the numerator and denominator.  If the
       numbers exceed 128 bits we fall back to 0 (the result is then
       inferred to be too large for uint64_t).  For larger seq_t types
       (arbitrary precision) we use the provided big_pow routine. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 bn = (unsigned __int128)n;
        unsigned __int128 base = 7 * bn * bn + 1;

        unsigned __int128 num, den;
        if (pow_uint128_overflow(base, n, &num) ||   /* (7n^2+1)^n */
            pow_uint128_overflow(bn, n, &den)) {      /* n^n        */
            return 0;                                /* overflow → no 64‑bit value */
        }

        unsigned __int128 res128 = num / den;
        return (seq_t)res128;
    } else {
        /* arbitrary‑precision build */
        seq_t base_big = (seq_t)n;
        base_big = 7 * base_big * base_big + 1;        /* 7n^2+1 */
        seq_t num_big = big_pow(base_big, n);          /* (7n^2+1)^n */
        seq_t den_big = big_pow(n, n);                 /* n^n      */
        return num_big / den_big;
    }
}
