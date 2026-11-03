/*
 * A078259  a(n) = denominator(N), where N = 0.135..(2n-1) is the
 *          concatenation of the first n odd numbers after decimal point.
 * https://oeis.org/A078259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, nonn
 * Formula:   a(n) = denominator of the decimal 0.135...(2n-1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Big‑int base: 1 000 000 000.   The largest value of n tested (≈1e4) needs
 * fewer than 7000 limbs in this base. */
#define BASE 1000000000U
#define MAX_LIMBS 7000

static const uint32_t POW10[10] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

static inline idx_t digits_of(idx_t v)
{
    if (v == 0) return 1;
    idx_t d = 0;
    while (v) { d++; v /= 10; }
    return d;
}

/* Multiply a big integer by a small integer (≤ BASE). */
static void bi_mul_small(uint32_t *limbs, size_t *len, uint32_t m)
{
    if (*len == 1 && limbs[0] == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint64_t prod = (uint64_t)limbs[i] * m + carry;
        limbs[i] = (uint32_t)(prod % BASE);
        carry    = prod / BASE;
    }
    while (carry) {
        limbs[(*len)++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Add a small integer (≤ 19999) to a big integer. */
static void bi_add_small(uint32_t *limbs, size_t *len, uint32_t v)
{
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < *len) {
        uint64_t sum = (uint64_t)limbs[i] + carry;
        limbs[i] = (uint32_t)(sum % BASE);
        carry    = sum / BASE;
        ++i;
    }
    if (carry) { limbs[(*len)++] = (uint32_t)carry; }
}

/* Divide the big integer by 2. */
static void bi_div2(uint32_t *limbs, size_t *len)
{
    uint64_t carry = 0;
    for (size_t i = *len; i-- > 0;) {
        uint64_t cur = carry * BASE + limbs[i];
        limbs[i] = (uint32_t)(cur >> 1);
        carry   = cur & 1;
    }
    while (*len > 1 && limbs[*len - 1] == 0) --(*len);
}

/* Divide the big integer by 5. */
static void bi_div5(uint32_t *limbs, size_t *len)
{
    uint64_t carry = 0;
    for (size_t i = *len; i-- > 0;) {
        uint64_t cur = carry * BASE + limbs[i];
        limbs[i] = (uint32_t)(cur / 5);
        carry    = cur % 5;
    }
    while (*len > 1 && limbs[*len - 1] == 0) --(*len);
}

#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Return 1 if the big integer is divisible by 5. */
static int bi_is_div5(const uint32_t *limbs, size_t len)
{
    return (limbs[0] % 5) == 0;
}

/* Return 1 if the big integer is even. */
static int bi_is_even(const uint32_t *limbs, size_t len)
{
    return (limbs[0] & 1) == 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* big‑int storage for the concatenated integer N */
    uint32_t limbs[MAX_LIMBS];
    size_t len = 1;
    limbs[0] = 0;

    idx_t total_digits = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t odd = 2 * i - 1;
        idx_t d   = digits_of(odd);
        total_digits += d;

        /* N = N * 10^d + odd */
        bi_mul_small(limbs, &len, POW10[d]);
        bi_add_small(limbs, &len, (uint32_t)odd);
    }

    /* count factors of 2 and 5 in N */
    idx_t e2 = 0;
    while (bi_is_even(limbs, len)) { bi_div2(limbs, &len); ++e2; }

    idx_t e5 = 0;
    while (bi_is_div5(limbs, len)) { bi_div5(limbs, &len); ++e5; }

    /* denominator = 2^(total_digits - e2) * 5^(total_digits - e5) */
    seq_t denom = ipow_idx((seq_t)2, total_digits - e2);
    seq_t pow5  = ipow_idx((seq_t)5, total_digits - e5);
    denom = seq_mul(denom, pow5);
    return denom;
}
