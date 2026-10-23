/*
 * A034074  a(0)=2; a(n) is the smallest k > a(n-1) such that the fractional
 *          part of k^(1/9) starts with n.
 * https://oeis.org/A034074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceiling((d + n/10^d)^9), d = 1 + floor(log10 n)
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_count.h"

/* 192‑bit unsigned integer, little‑endian: w[0] least‑significant. */
typedef struct {
    uint64_t w[3];
} Uint192;

/* Zero constant */
static const Uint192 U192_ZERO = {{0,0,0}};

/* Compare two Uint192 values: 1 if a>b, -1 if a<b, 0 if equal. */
static int cmp_uint192(const Uint192 *a, const Uint192 *b)
{
    for (int i = 2; i >= 0; --i) {
        if (a->w[i] > b->w[i]) return 1;
        if (a->w[i] < b->w[i]) return -1;
    }
    return 0;
}

/* Bit length of Uint192 (0 if zero). */
static int bitlen_uint192(const Uint192 *a)
{
    for (int i = 2; i >= 0; --i) {
        if (a->w[i]) {
#if defined(__GNUC__) || defined(__clang__)
            return i * 64 + (64 - __builtin_clzll(a->w[i]));
#else
            /* Portable fallback */
            int b = a->w[i];
            int len = 1;
            while (b >>= 1) ++len;
            return i * 64 + len;
#endif
        }
    }
    return 0;
}

/* Multiply Uint192 by small unsigned 64‑bit value (m < 2^32). */
static void mul_uint192_small(Uint192 *a, uint64_t m)
{
    unsigned __int128 carry = 0;
    for (int i = 0; i < 3; ++i) {
        unsigned __int128 tmp = (unsigned __int128)a->w[i] * m + carry;
        a->w[i] = (uint64_t)tmp;
        carry   = tmp >> 64;
    }
}

/* Subtract b from a (assumes a >= b). */
static void sub_uint192(Uint192 *a, const Uint192 *b)
{
    unsigned __int128 borrow = 0;
    for (int i = 0; i < 3; ++i) {
        unsigned __int128 di = (unsigned __int128)b->w[i] + borrow;
        if (a->w[i] < di) {
            a->w[i] = a->w[i] - di;
            borrow  = 1;
        } else {
            a->w[i] = a->w[i] - di;
            borrow  = 0;
        }
    }
}

/* Right shift Uint192 by one bit. */
static void shift_right1_uint192(Uint192 *a)
{
    uint64_t carry = 0;
    for (int i = 2; i >= 0; --i) {
        uint64_t newcarry = a->w[i] & 1ULL;
        a->w[i] = (a->w[i] >> 1) | (carry << 63);
        carry   = newcarry;
    }
}

/* Shift Uint192 left by k bits (k <= 192). */
static void shift_left_uint192(const Uint192 *src, int k, Uint192 *dst)
{
    int word_shift = k / 64;
    int bit_shift  = k % 64;
    dst->w[0] = dst->w[1] = dst->w[2] = 0ULL;

    for (int i = 0; i < 3; ++i) {
        if (src->w[i] == 0) continue;
        int target = i + word_shift;
        if (target > 2) break;
        dst->w[target] |= src->w[i] << bit_shift;
        if (bit_shift && target + 1 <= 2) {
            dst->w[target + 1] |= src->w[i] >> (64 - bit_shift);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;

    /* Number of decimal digits of n. */
    idx_t d = digit_count((uint64_t)n);      /* d ≥ 1 for n > 0 */

    /* 10^d as uint64_t (fits in 32 bits for d ≤ 5). */
    uint64_t pow10d = 1;
    for (int i = 0; i < d; ++i) pow10d *= 10ULL;

    /* x = d * 10^d + n.  This fits into 64 bits. */
    uint64_t x = (uint64_t)d * pow10d + (uint64_t)n;

    /* Compute numerator = x^9 as Uint192. */
    Uint192 num = {{x, 0, 0}};
    for (int i = 1; i < 9; ++i) {
        mul_uint192_small(&num, x);
    }

    /* Compute denominator = (10^d)^9 = 10^(9*d). */
    Uint192 den = {{1, 0, 0}};
    int e = 9 * (int)d;          /* 1 ≤ e ≤ 45 */
    for (int i = 0; i < e; ++i) {
        mul_uint192_small(&den, 10);
    }

    /* Divide num by den: obtain quotient q and remainder rem. */
    Uint192 cur;
    int shift_max = bitlen_uint192(&num) - bitlen_uint192(&den);
    shift_left_uint192(&den, shift_max, &cur);

    uint64_t q = 0;
    for (int shift = shift_max; shift >= 0; --shift) {
        if (cmp_uint192(&cur, &num) <= 0) {
            sub_uint192(&num, &cur);
            q += (uint64_t)1 << shift;
        }
        shift_right1_uint192(&cur);
    }

    /* The remaining num is the remainder. */
    bool has_remainder = (cmp_uint192(&num, &U192_ZERO) != 0);
    uint64_t result = q + (has_remainder ? 1 : 0);

    return (seq_t)result;
}
