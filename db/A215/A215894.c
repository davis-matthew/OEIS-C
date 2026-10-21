/*
 * A215894  a(n) = floor(2^n / n^k), where k is the largest integer such that
 *          2^n >= n^k.
 * https://oeis.org/A215894
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(2^n / n^k), where k = max { m : n^m ≤ 2^n } (offset
 *            2)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define BIGINT_WORDS 400          /* enough for 2^10000   (≈32 000 bits) */

typedef struct {
    uint32_t w[BIGINT_WORDS];
    size_t len;
} BigInt;

/* initialise BigInt to a 64‑bit value */
static void bigint_set_uint(BigInt *a, uint64_t v)
{
    a->len = 0;
    if (v == 0) return;
    a->w[0] = (uint32_t)v;
    a->w[1] = (uint32_t)(v >> 32);
    a->len = (a->w[1] != 0) ? 2 : 1;
}

/* shift left by one bit (2^1 multiplication) */
static void bigint_shift_left1(BigInt *a)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint32_t new_carry = a->w[i] >> 31;
        a->w[i] = (a->w[i] << 1) | carry;
        carry = new_carry;
    }
    if (carry) {
        a->w[a->len++] = carry;
    }
}

/* multiply BigInt a by a small unsigned int m, store result in res (does not modify a) */
static void bigint_mul_small(const BigInt *a, uint32_t m, BigInt *res)
{
    if (m == 0 || a->len == 0) {
        res->len = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->w[i] * m + carry;
        res->w[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    res->len = a->len;
    if (carry) {
        res->w[res->len++] = (uint32_t)carry;
    }
}

/* compare two BigInts: return -1 if a<b, 0 if a==b, 1 if a>b */
static int bigint_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len < b->len) return -1;
    if (a->len > b->len) return 1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->w[i] < b->w[i]) return -1;
        if (a->w[i] > b->w[i]) return 1;
    }
    return 0;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* definition valid only for n≥2 */

    /* 2^n as BigInt */
    BigInt two_pow;
    bigint_set_uint(&two_pow, 1);
    for (idx_t i = 0; i < n; ++i) bigint_shift_left1(&two_pow);

    /* find largest k with n^k ≤ 2^n */
    BigInt power;                 /* n^k */
    bigint_set_uint(&power, 1);   /* starts as n^0 */
    BigInt tmp;
    idx_t k = 0;
    for (;;) {
        bigint_mul_small(&power, (uint32_t)n, &tmp);
        int cmp_res = bigint_cmp(&tmp, &two_pow);
        if (cmp_res <= 0) {
            /* tmp becomes new power */
            power.len = tmp.len;
            for (size_t i = 0; i < tmp.len; ++i) power.w[i] = tmp.w[i];
            ++k;
        } else break;
    }

    /* binary search for a(n) = floor(2^n / n^k) */
    idx_t low = 1, high = n - 1, ans = 1;
    BigInt prod;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        bigint_mul_small(&power, (uint32_t)mid, &prod);
        if (bigint_cmp(&prod, &two_pow) <= 0) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return (seq_t)ans;
}
