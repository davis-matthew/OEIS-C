/*
 * A176439  Decimal expansion of (7+sqrt(53))/2.
 * https://oeis.org/A176439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor( ((7+sqrt(53))/2) * 10^(n-1) ) mod 10 (decimal
 *            expansion of (7+sqrt(53))/2)
 * Range:     u8 n <= 9843, u16 n <= 9843, u32 n <= 9843, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define BASE 1000000000U          /* 1e9 */
#define MAX_LIMBS 1500            /* enough for 10000 decimal digits */

/* big integer in little‑endian base BASE */
typedef struct {
    uint32_t d[MAX_LIMBS];
    size_t len;          /* number of used limbs, 0 means zero */
} BigInt;

static void bi_zero(BigInt *a) { a->len = 0; }

static void bi_from_uint32(BigInt *a, uint32_t v) {
    if (v == 0) {
        a->len = 0;
    } else {
        a->d[0] = v;
        a->len = 1;
    }
}

static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static void bi_add_small(BigInt *a, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        ++i;
    }
    if (carry) {
        a->d[a->len++] = (uint32_t)carry;
    }
}

static void bi_mul_small(BigInt *res, const BigInt *src, uint32_t m) {
    if (m == 0 || src->len == 0) {
        res->len = 0;
        return;
    }
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < src->len; ++i) {
        uint64_t prod = (uint64_t)src->d[i] * m + carry;
        res->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) {
        res->d[i] = (uint32_t)carry;
        res->len = i + 1;
    } else {
        res->len = i;
    }
}

static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

static void bi_sub_inplace(BigInt *a, const BigInt *b) { /* assumes a>=b */
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i] - borrow -
                       (i < b->len ? b->d[i] : 0);
        if ((int64_t)sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

static void bi_div2(BigInt *a) { /* a = a/2 */
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0;) {
        uint64_t cur = a->d[i] + rem * BASE;
        a->d[i] = (uint32_t)(cur / 2);
        rem = cur % 2;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)7;

    idx_t k = n - 1;                 /* number of fractional digits needed */

    /* Big integers used in the algorithm */
    BigInt remainder, root, t, cand, tmp, pow10, mult;
    bi_zero(&remainder);
    bi_zero(&root);

    /* Iterate over the groups: first group is 53, then k groups of 00 */
    for (idx_t g = 0; g < k + 1; ++g) {
        /* remainder = remainder * 100 + group */
        bi_mul_small(&remainder, &remainder, 100);
        if (g == 0)
            bi_add_small(&remainder, 53);

        /* t = root * 20 */
        bi_mul_small(&t, &root, 20);

        int chosen = 0;
        /* find largest d such that (t + d)*d <= remainder */
        for (int d = 9; d >= 0; --d) {
            bi_copy(&cand, &t);
            bi_add_small(&cand, (uint32_t)d);
            bi_mul_small(&cand, &cand, (uint32_t)d);
            if (bi_cmp(&cand, &remainder) <= 0) {
                chosen = d;
                bi_sub_inplace(&remainder, &cand);
                break;
            }
        }

        /* root = root * 10 + chosen */
        bi_mul_small(&root, &root, 10);
        bi_add_small(&root, (uint32_t)chosen);
    }

    /* Compute 7 * 10^k */
    bi_from_uint32(&pow10, 1);
    for (idx_t i = 0; i < k; ++i)
        bi_mul_small(&pow10, &pow10, 10);
    bi_mul_small(&mult, &pow10, 7);

    /* rem = root - 7*10^k */
    bi_sub_inplace(&root, &mult);     /* now root holds rem */

    /* floor(rem/2) */
    bi_div2(&root);

    /* last decimal digit of floor(rem/2) */
    int digit = (root.len == 0) ? 0 : (int)(root.d[0] % 10);
    return (seq_t)digit;
}
