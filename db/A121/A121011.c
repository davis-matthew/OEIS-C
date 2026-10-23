/*
 * A121011  Denominators of partial alternating sums of Catalan numbers scaled
 *          by powers of 1/(5*8^2) = 1/320.
 * https://oeis.org/A121011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of r(n) = ∑_{k=0..n} (-1)^k * Catalan(k) /
 *            320^k a(n) = 320^n / gcd( P_n , 320^n ) with P_n = ∑_{k=0..n}
 *            (-1)^k * Catalan(k) * 320^{n-k}
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 3, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/pow_seq.h"   /* seq_t pow_seq(seq_t base, int exp) */

#define MAX_BIGINT_LIMBS 4000   /* enough for n≈10000 (≈2600 limbs) */

typedef struct {
    uint32_t limb[MAX_BIGINT_LIMBS];
    size_t   len;
} BigInt;

/* initialize to zero */
static inline void bi_set_zero(BigInt *a) {
    a->len = 0;
}

/* initialize to one */
static inline void bi_set_one(BigInt *a) {
    a->len = 1;
    a->limb[0] = 1;
}

/* copy */
static inline void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    memcpy(dst->limb, src->limb, src->len * sizeof(uint32_t));
}

/* remove leading zero limbs */
static inline void bi_normalize(BigInt *a) {
    while (a->len && a->limb[a->len-1] == 0) a->len--;
}

/* add src to dst: dst += src */
static void bi_add(BigInt *dst, const BigInt *src) {
    size_t i, maxlen = dst->len > src->len ? dst->len : src->len;
    uint64_t carry = 0;
    for (i = 0; i < maxlen; ++i) {
        uint64_t sum = carry;
        if (i < dst->len) sum += dst->limb[i];
        if (i < src->len) sum += src->limb[i];
        dst->limb[i] = (uint32_t)sum;
        carry = sum >> 32;
    }
    if (carry) {
        dst->limb[maxlen] = (uint32_t)carry;
        dst->len = maxlen + 1;
    } else {
        dst->len = maxlen;
    }
}

/* subtract src from dst: dst -= src (assumes dst ≥ src) */
static void bi_sub(BigInt *dst, const BigInt *src) {
    size_t i;
    uint64_t borrow = 0;
    for (i = 0; i < dst->len; ++i) {
        uint64_t sub = (uint64_t)dst->limb[i] - borrow;
        if (i < src->len) sub -= src->limb[i];
        if ((int64_t)sub < 0) {
            sub += ((uint64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        dst->limb[i] = (uint32_t)sub;
    }
    bi_normalize(dst);
}

/* multiply by small integer m (32‑bit) */
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0) { bi_set_zero(a); return; }
    if (m == 1) return;
    size_t i;
    uint64_t carry = 0;
    for (i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)prod;
        carry = prod >> 32;
    }
    if (carry) {
        if (a->len < MAX_BIGINT_LIMBS) {
            a->limb[a->len] = (uint32_t)carry;
            a->len++;
        }
    }
}

/* divide by small integer d (32‑bit), return remainder; modifies a */
static void bi_div_small(BigInt *a, uint32_t d, uint32_t *rem_out) {
    uint64_t rem = 0;
    size_t i;
    for (i = a->len; i-- > 0;) {
        uint64_t cur = (rem << 32) | a->limb[i];
        a->limb[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    *rem_out = (uint32_t)rem;
    bi_normalize(a);
}

/* compute v2 or v5 exponent of big integer a (a is destroyed) */
static uint64_t bi_val_exponent(BigInt *a, uint32_t factor) {
    uint64_t val = 0;
    uint32_t rem;
    while (1) {
        bi_div_small(a, factor, &rem);
        if (rem == 0) val++;
        else break;
    }
    return val;
}

seq_t a(idx_t n) {
    /* Big integer used to hold P_n and Catalan numbers */
    BigInt P, C, tmp;
    bi_set_one(&P);   /* P_0 = 1 */
    bi_set_one(&C);   /* C_0 = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        /* C_i = C_{i-1} * (4*i-2) / (i+1) */
        bi_mul_small(&C, (uint32_t)(4 * i - 2));
        uint32_t rem;
        bi_div_small(&C, (uint32_t)(i + 1), &rem);  /* exact division; rem == 0 */

        /* P_i = 320 * P_{i-1} + (-1)^i * C_i */
        bi_mul_small(&P, 320);

        if ((i & 1) == 0) {          /* even i: add C_i */
            bi_add(&P, &C);
        } else {                      /* odd i: subtract C_i */
            bi_sub(&P, &C);
        }
    }

    /* compute exponent of 2 in P_n */
    bi_copy(&tmp, &P);
    uint64_t v2 = bi_val_exponent(&tmp, 2);

    /* compute exponent of 5 in P_n */
    bi_copy(&tmp, &P);
    uint64_t v5 = bi_val_exponent(&tmp, 5);

    /* calculate final exponents for denominator */
    uint64_t exp2 = 6 * n - v2;   /* 6n - v2(P_n) */
    uint64_t exp5 = n - v5;       /* n  - v5(P_n) */

    /* compute 2^exp2 * 5^exp5 */
    seq_t res = pow_seq(2, (int)exp2);
    res *= pow_seq(5, (int)exp5);
    return res;
}
