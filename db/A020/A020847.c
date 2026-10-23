/*
 * A020847  Decimal expansion of 1/sqrt(90) = sqrt(10)/30.
 * https://oeis.org/A020847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor( sqrt(10) * 10^n / 3 ) mod 10 This implementation
 *            computes the nth digit after the decimal point of 1/sqrt(90) =
 *            sqrt(10)/30 using integer arithmetic only
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define BASE 1000000000U   /* 10^9 is a suitable base for limb storage */
#define MAX_LIMBS 1500     /* enough for 10001 decimal digits (≈1112 limbs) */

/* Simple big integer in base 1e9, little-endian */
typedef struct {
    size_t len;                               /* number of used limbs, len==0 -> 0 */
    uint32_t d[MAX_LIMBS];
} BigInt;

/* Utility: set to zero */
static void bi_zero(BigInt *a) {
    a->len = 0;
}

/* Utility: set to small integer (value fits in uint32_t) */
static void bi_set(BigInt *a, uint32_t v) {
    if (v == 0) {
        a->len = 0;
    } else {
        a->len = 1;
        a->d[0] = v;
    }
}

/* Utility: copy */
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) {
        dst->d[i] = src->d[i];
    }
}

/* Utility: trim leading zero limbs */
static void bi_normalize(BigInt *a) {
    while (a->len > 0 && a->d[a->len - 1] == 0) {
        a->len--;
    }
}

/* Compare a and b: return -1 if a<b, 0 if a==b, 1 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len < b->len) return -1;
    if (a->len > b->len) return 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] < b->d[i]) return -1;
        if (a->d[i] > b->d[i]) return 1;
    }
    return 0;
}

/* Addition of a small integer (0 <= v < BASE) to BigInt a */
static void bi_add_small(BigInt *a, uint32_t v) {
    if (v == 0) return;
    if (a->len == 0) {
        a->len = 1;
        a->d[0] = v;
        return;
    }
    uint64_t sum = (uint64_t)a->d[0] + v;
    a->d[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        uint64_t t = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(t % BASE);
        carry = t / BASE;
        ++i;
    }
    if (carry) {
        if (a->len >= MAX_LIMBS) return; /* overflow guard */
        a->d[a->len] = (uint32_t)carry;
        a->len++;
    }
}

/* Multiply BigInt a by a small integer m (m < 1<<16 for safety) */
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 1 || a->len == 0) return;
    if (m == 0) {
        a->len = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        if (a->len >= MAX_LIMBS) return; /* overflow guard */
        a->d[a->len] = (uint32_t)(carry % BASE);
        carry /= BASE;
        a->len++;
    }
}

/* Subtract b from a (a >= b), result in res = a - b */
static void bi_sub(BigInt *res, const BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t aval = a->d[i];
        int64_t bval = (i < b->len) ? b->d[i] : 0;
        int64_t sub = aval - bval - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res->d[i] = (uint32_t)sub;
    }
    res->len = a->len;
    bi_normalize(res);
}

/* Divide BigInt a by small integer d (d > 0), store quotient in q,
   remainder in rem (rem may be NULL). */
static void bi_div_small(const BigInt *a, uint32_t d, BigInt *q, uint32_t *rem) {
    if (a->len == 0) {
        q->len = 0;
        if (rem) *rem = 0;
        return;
    }
    q->len = a->len;
    uint64_t r = 0;
    for (size_t ii = a->len; ii-- > 0; ) {
        uint64_t cur = r * BASE + a->d[ii];
        uint32_t quo = (uint32_t)(cur / d);
        r = cur % d;
        q->d[ii] = quo;
    }
    bi_normalize(q);
    if (rem) *rem = (uint32_t)r;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* We compute R = floor(sqrt(10) * 10^n) using digit-by-digit sqrt.
       The answer is ((R / 3) % 10). */
    BigInt res, cur, temp, q;
    bi_zero(&res);
    bi_zero(&cur);

    /* Groups: first group is 10, then n groups of 0.
       Number of groups = n + 1. */
    for (idx_t i = 0; i <= n; ++i) {
        uint32_t group = (i == 0) ? 10U : 0U;

        /* cur = cur * 100 + group */
        bi_mul_small(&cur, 100U);
        bi_add_small(&cur, group);

        /* Find largest digit d (0-9) such that (20*res + d) * d <= cur */
        int best = 0;
        for (int d = 9; d >= 0; --d) {
            bi_copy(&temp, &res);
            bi_mul_small(&temp, 20U);
            bi_add_small(&temp, (uint32_t)d);
            bi_mul_small(&temp, (uint32_t)d);
            if (bi_cmp(&temp, &cur) <= 0) {
                best = d;
                break;
            }
        }

        /* cur -= temp */
        bi_sub(&cur, &cur, &temp);

        /* res = res * 10 + best */
        bi_mul_small(&res, 10U);
        bi_add_small(&res, (uint32_t)best);
    }

    /* Divide res by 3 to obtain floor(res / 3) */
    bi_div_small(&res, 3U, &q, NULL);

    /* The desired digit is the least significant decimal digit of q.
       Since BASE = 1e9 is divisible by 10, q.d[0] % 10 gives that digit. */
    uint32_t digit = 0;
    if (q.len > 0) {
        digit = q.d[0] % 10U;
    }
    return (seq_t)digit;
}
