/*
 * A034075  a(0)=2; a(n) is the smallest k > a(n-1) such that the fractional
 *          part of k^(1/10) starts with n.
 * https://oeis.org/A034075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil( (d + n / 10^d)^10 ) where d = 1 + floor(log10 n)
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define BI_BASE 1000000000U       /* 10^9, used as base for big integer limbs */

typedef struct {
    unsigned int len;           /* number of used limbs (little‑endian) */
    uint32_t d[20];             /* limbs, each < BI_BASE */
} bi_t;

/* initialise big integer with an unsigned integer value */
static void bi_set_uint32(bi_t *bi, uint32_t v)
{
    bi->len = 1;
    bi->d[0] = v;
}

/* multiply big integer by a 32‑bit integer */
static void bi_mul_uint32(bi_t *bi, uint32_t m)
{
    if (m == 0) {
        bi->len = 1;
        bi->d[0] = 0;
        return;
    }
    __uint128_t carry = 0;
    for (unsigned int i = 0; i < bi->len; ++i) {
        __uint128_t prod = (__uint128_t)bi->d[i] * m + carry;
        bi->d[i] = (uint32_t)(prod % BI_BASE);
        carry   = prod / BI_BASE;
    }
    while (carry) {
        bi->d[bi->len++] = (uint32_t)(carry % BI_BASE);
        carry /= BI_BASE;
    }
}

/* divide big integer by a 32‑bit integer; return remainder */
static uint32_t bi_div_uint32_mod(bi_t *bi, uint32_t m)
{
    __uint128_t carry = 0;
    for (int i = (int)bi->len - 1; i >= 0; --i) {
        __uint128_t cur = carry * BI_BASE + bi->d[i];
        bi->d[i] = (uint32_t)(cur / m);
        carry   = cur % m;
    }
    /* strip leading zeros */
    while (bi->len > 1 && bi->d[bi->len - 1] == 0)
        --bi->len;
    return (uint32_t)carry;
}

/* convert a big integer to seq_t (works for uint8_t .. arbitrary precision) */
static seq_t bi_to_seq_t(const bi_t *bi)
{
    seq_t v = 0;
    for (int i = (int)bi->len - 1; i >= 0; --i) {
        v = v * (seq_t)BI_BASE + (seq_t)bi->d[i];
    }
    return v;
}

/* compute 10^e, 0 <= e <= 10 (fits in 64‑bit) */
static uint64_t pow10_u64(unsigned int e)
{
    uint64_t r = 1;
    for (unsigned int i = 0; i < e; ++i)
        r *= 10;
    return r;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;                 /* a(0) = 2 */

    /* d = number of decimal digits of n (1 + floor(log10 n)) */
    idx_t tmp = n;
    unsigned int d = 1;
    while (tmp >= 10) {
        tmp /= 10;
        ++d;
    }

    /* t = d * 10^d + n */
    uint64_t pow10d = pow10_u64(d);
    uint64_t t64    = (uint64_t)d * pow10d + (uint64_t)n;   /* t fits in 64‑bit */
    uint32_t t      = (uint32_t)t64;                       /* t <= ~510000 */

    /* compute t^10 as a big integer */
    bi_t bi;
    bi_set_uint32(&bi, t);
    for (int i = 1; i < 10; ++i)
        bi_mul_uint32(&bi, t);

    /* divide by 10^(10*d) in 10*d repeated steps of division by 10;
       detect if any remainder occurs */
    bool rem_nonzero = false;
    unsigned int div_steps = 10 * d;
    for (unsigned int i = 0; i < div_steps; ++i) {
        uint32_t rem = bi_div_uint32_mod(&bi, 10);
        if (rem != 0)
            rem_nonzero = true;
    }

    /* bi now holds floor(t^10 / 10^(10*d)) */
    seq_t result = bi_to_seq_t(&bi);
    if (rem_nonzero)
        result += (seq_t)1;                /* ceil */

    return result;
}
