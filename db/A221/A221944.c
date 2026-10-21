/*
 * A221944  Floor(sqrt(2*3^n)).
 * https://oeis.org/A221944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2*3^n))
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 80, u128 n <= 40,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

/* Compute a(n) = ⌊√(2·3ⁿ)⌋, valid for any seq_t size. */
seq_t a(idx_t n)
{
    /* If seq_t is large enough (≥128 bits) we can use the arbitrary‑precision
       helpers directly. */
    if (sizeof(seq_t) >= 16) {
        /* big_pow handles arbitrary precision; 3 is the base, n is the exponent. */
        seq_t p = big_pow((seq_t)3, (seq_t)n);
        seq_t val = p * (seq_t)2;
        return floor_sqrt(val);
    }

    /* For smaller seq_t (e.g. 64 bits) use 128‑bit arithmetic to avoid
       overflow when computing 3ⁿ and the square root.  This handles all
       values that fit into the target type. */
    __uint128_t power = 1;
    for (idx_t i = 0; i < n; ++i) {
        power *= 3;
    }
    power *= 2;                     /* 2·3ⁿ */

    /* Integer square root of a 128‑bit number using binary search. */
    __uint128_t low = 0;
    __uint128_t high = (__uint128_t)1 << 64;  /* sqrt of any value < 2^128 */
    while (low < high) {
        __uint128_t mid = (low + high + 1) >> 1;
        if (mid * mid <= power) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }

    return (seq_t)low;
}
