/*
 * A034312  Decimal part of a(n)^(1/6) starts with reversal of its integer
 *          part: first term of runs.
 * https://oeis.org/A034312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal integer x such that the sixth root of x has
 *            integer part n+1 and its decimal part starts with the reversal
 *            of that integer part
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: count decimal digits of x (x > 0) */
static inline int digits(idx_t x) {
    int d = 0;
    while (x) { x /= 10; ++d; }
    return d > 0 ? d : 1;
}

/* Helper: reverse decimal digits of x */
static inline idx_t rev(idx_t x) {
    idx_t y = 0;
    do { y = y * 10 + (x % 10); x /= 10; } while (x);
    return y;
}

/* Small big integer type: 256‑bit unsigned, 4 limbs of 64 bits */
typedef struct { uint64_t limb[4]; } big256_t;

/* Initialize to zero */
static inline void big256_zero(big256_t *b) { b->limb[0] = b->limb[1] = b->limb[2] = b->limb[3] = 0; }
/* Set from 64‑bit value */
static inline void big256_from_u64(big256_t *b, uint64_t v) { big256_zero(b); b->limb[0] = v; }
/* Multiply big256 by a 64‑bit number */
static void big256_mul_u64(big256_t *b, uint64_t m) {
    __uint128_t carry = 0;
    for (int i = 0; i < 4; ++i) {
        __uint128_t prod = (__uint128_t)b->limb[i] * m + carry;
        b->limb[i] = (uint64_t)prod;
        carry = prod >> 64;
    }
}

/* Divide a 256‑bit big integer (b) by a 128‑bit divisor.
   Returns floor = b/divisor and remainder.  Assumes divisor > 0. */
static void big256_div_u128(const big256_t *b, __uint128_t divisor,
                            __uint128_t *floor, __uint128_t *remainder) {
    __uint128_t rem = 0;
    __uint128_t quot = 0;
    for (int i = 3; i >= 0; --i) {
        __uint128_t cur = (rem << 64) | b->limb[i];
        uint64_t q = (uint64_t)(cur / divisor);
        rem = cur % divisor;
        quot = (quot << 64) | q;
    }
    *floor = quot;
    *remainder = rem;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* k = integer part of sixth root */
    idx_t k = (idx_t)n + 1ULL;

    /* number of decimal digits of k */
    int len = digits(k);

    /* rev(k) */
    idx_t r = rev(k);

    /* pow10_len = 10^len (fits in 64 bits for len ≤ 5) */
    uint64_t pow10_len = 1;
    for (int i = 0; i < len; ++i) pow10_len *= 10ULL;

    /* N = k * 10^len + r  (fits in 32 bits, actually < 1e9) */
    uint64_t N = (uint64_t)k * pow10_len + r;

    /* divisor D = 10^(6*len) (fits in 128 bits) */
    int pow_exp = 6 * len;
    __uint128_t D = 1;
    for (int i = 0; i < pow_exp; ++i) D *= 10;

    /* Compute N^6 as a 256‑bit integer */
    big256_t num; big256_from_u64(&num, 1);
    for (int i = 0; i < 6; ++i) big256_mul_u64(&num, N);

    /* Divide by D : floor = num / D, rem = num % D */
    __uint128_t floor_div, rem;
    big256_div_u128(&num, D, &floor_div, &rem);

    /* ceil = floor + (rem > 0) */
    __uint128_t ceil_val = floor_div + (rem != 0 ? 1 : 0);

    /* return as seq_t (fits in all builds for the tested n) */
    return (seq_t)ceil_val;
}
