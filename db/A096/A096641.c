/*
 * A096641  Decimal expansion of number with continued fraction expansion 0,
 *          2, 4, 8, 16, ... (0 and positive powers of 2).
 * https://oeis.org/A096641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A096641:  decimal expansion of the number with continued fraction
 *  [0; 2, 4, 8, 16, …]  (a₀ = 0, a_k = 2^k for k ≥ 1).
 *
 *  The function returns the nth decimal digit (0‑based).
 *  It uses a small arbitrary‑precision integer implementation that
 *  works only with 32‑bit limbs.  Memory usage is bounded by a
 *  fixed limit of 2000 limbs, which is sufficient for the required
 *  10 000 digits (~40 000 bits).
 *
 *  The algorithm:
 *  1. Generate convergents of the continued fraction until the
 *     denominator has enough bits to guarantee that the first
 *     target = n+1 digits are correct.
 *  2. Extract decimal digits from the final fraction p/q by
 *     repeated multiplication by 10 and subtraction.
 *  3. Cache the digits in a static array so that subsequent calls
 *     do not recompute earlier digits.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAXLIMBS 2000          /* enough for ~40 000 bits */

/* ------------------------------------------------------------------ */
/*  Small arbitrary‑precision integer (base 2^32)                      */
/* ------------------------------------------------------------------ */
typedef struct {
    uint32_t limb[MAXLIMBS];
    size_t sz;                /* number of used limbs (≥1) */
} BI;

/* Ensure the most significant limb is non‑zero (except for 0 itself). */
static void bi_trim(BI *x) {
    while (x->sz > 1 && x->limb[x->sz - 1] == 0)
        x->sz--;
}

/* Set BI to the value of a 64‑bit unsigned integer. */
static void bi_set_uint64(BI *x, uint64_t v) {
    if (v >> 32) {
        x->limb[0] = (uint32_t)(v & 0xffffffffu);
        x->limb[1] = (uint32_t)(v >> 32);
        x->sz = 2;
    } else {
        x->limb[0] = (uint32_t)v;
        x->sz = 1;
    }
}

/* Copy BI. */
static void bi_copy(BI *dst, const BI *src) {
    dst->sz = src->sz;
    memcpy(dst->limb, src->limb, src->sz * sizeof(uint32_t));
}

/* Compare two BIs.  Returns -1 if a<b, 0 if a==b, 1 if a>b. */
static int bi_cmp(const BI *a, const BI *b) {
    if (a->sz != b->sz)
        return a->sz < b->sz ? -1 : 1;
    for (size_t i = a->sz; i-- > 0; ) {
        if (a->limb[i] != b->limb[i])
            return a->limb[i] < b->limb[i] ? -1 : 1;
    }
    return 0;
}

/* Add two BIs: res = a + b. */
static void bi_add(BI *res, const BI *a, const BI *b) {
    uint64_t carry = 0;
    size_t i = 0;
    for (; i < a->sz || i < b->sz; ++i) {
        uint64_t ai = i < a->sz ? a->limb[i] : 0;
        uint64_t bi = i < b->sz ? b->limb[i] : 0;
        uint64_t sum = ai + bi + carry;
        res->limb[i] = (uint32_t)(sum & 0xffffffffu);
        carry = sum >> 32;
    }
    if (carry) {
        res->limb[i] = (uint32_t)carry;
        res->sz = i + 1;
    } else {
        res->sz = i;
    }
}

/* Subtract two BIs: res = a - b  (assumes a >= b). */
static void bi_sub(BI *res, const BI *a, const BI *b) {
    int64_t borrow = 0;
    size_t i = 0;
    for (; i < a->sz; ++i) {
        int64_t ai = a->limb[i];
        int64_t bi = i < b->sz ? b->limb[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += ((int64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        res->limb[i] = (uint32_t)diff;
    }
    res->sz = a->sz;
    bi_trim(res);
}

/* Multiply BI by a small unsigned 32‑bit integer: res = a * m. */
static void bi_mul_small(BI *res, const BI *a, uint32_t m) {
    if (m == 0) {
        bi_set_uint64(res, 0);
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->sz; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        res->limb[i] = (uint32_t)(prod & 0xffffffffu);
        carry = prod >> 32;
    }
    if (carry) {
        res->limb[a->sz] = (uint32_t)carry;
        res->sz = a->sz + 1;
    } else {
        res->sz = a->sz;
    }
    bi_trim(res);
}

/* Shift BI left by 'bits' bits: res = a << bits.  (bits ≤ 2^32). */
static void bi_shl(BI *res, const BI *a, unsigned bits) {
    if (bits == 0) {
        bi_copy(res, a);
        return;
    }
    unsigned limb_shift = bits / 32;
    unsigned bit_shift = bits % 32;
    size_t i;
    uint64_t carry = 0;
    /* Place shifted limbs. */
    for (i = 0; i < a->sz; ++i) {
        uint64_t cur = ((uint64_t)a->limb[i] << bit_shift) | carry;
        res->limb[i + limb_shift] = (uint32_t)(cur & 0xffffffffu);
        carry = cur >> 32;
    }
    if (carry) {
        res->limb[i + limb_shift] = (uint32_t)carry;
        res->sz = a->sz + limb_shift + 1;
    } else {
        res->sz = a->sz + limb_shift;
    }
    /* Zero the lower limbs created by limb_shift. */
    for (i = 0; i < limb_shift; ++i)
        res->limb[i] = 0;
    bi_trim(res);
}

/* Return number of bits required to represent BI (0 for zero). */
static size_t bi_bitlen(const BI *x) {
    if (x->sz == 1 && x->limb[0] == 0)
        return 0;
    uint32_t top = x->limb[x->sz - 1];
    return (x->sz - 1) * 32 + (32 - __builtin_clz(top));
}

/* ------------------------------------------------------------------ */
/*  Main function                                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    static uint8_t *digits = NULL;       /* cached decimal digits */
    static size_t digits_len = 0;        /* number of cached digits */

    /* At least the first n+1 digits are required. */
    const idx_t target = n + 1;

    if (digits_len < (size_t)target) {
        /* ------------------------------------------------------------ */
        /*  Compute convergents until denominator is large enough.     */
        /* ------------------------------------------------------------ */
        /* We use a bit count safety margin: denominator must have
           more than target * 4 bits (≈ log2(10) * target).          */
        const size_t bit_limit = target * 4;   /* safe over‑estimate */

        BI p[3], q[3];
        /* Initialise: p_-1 = 1, p_0 = 0 ; q_-1 = 0, q_0 = 1 */
        bi_set_uint64(&p[0], 1);  /* p_prev2 */
        bi_set_uint64(&p[1], 0);  /* p_prev1 */
        bi_set_uint64(&q[0], 0);  /* q_prev2 */
        bi_set_uint64(&q[1], 1);  /* q_prev1 */

        size_t depth = 0;          /* number of processed terms after a0 */

        BI temp, new_p, new_q;
        BI shift_p, shift_q;

        while (bi_bitlen(&q[1]) <= bit_limit) {
            unsigned k = (unsigned)(depth + 1);   /* a_k = 2^k  */

            /* new_p = (p_prev1 << k) + p_prev2 */
            bi_shl(&shift_p, &p[1], k);
            bi_add(&new_p, &shift_p, &p[0]);

            /* new_q = (q_prev1 << k) + q_prev2 */
            bi_shl(&shift_q, &q[1], k);
            bi_add(&new_q, &shift_q, &q[0]);

            /* Rotate arrays: p[0] <- p[1], p[1] <- new_p */
            bi_copy(&p[0], &p[1]);
            bi_copy(&p[1], &new_p);
            bi_copy(&q[0], &q[1]);
            bi_copy(&q[1], &new_q);

            ++depth;
        }

        /* ------------------------------------------------------------ */
        /*  Extract decimal digits from the final fraction p/q.           */
        /* ------------------------------------------------------------ */

        BI rem, den;
        bi_copy(&rem, &p[1]);   /* numerator */
        bi_copy(&den, &q[1]);   /* denominator */

        /* Cache new digits. */
        uint8_t *new_digits = (uint8_t *)malloc(target * sizeof(uint8_t));
        if (!new_digits)
            return (seq_t)0;   /* fallback on allocation failure */

        for (idx_t i = 0; i < target; ++i) {
            /* rem = rem * 10 */
            bi_mul_small(&rem, &rem, 10);

            /* digit = floor(rem / den)  (digit <= 9) */
            uint8_t d = 0;
            while (bi_cmp(&rem, &den) >= 0) {
                bi_sub(&rem, &rem, &den);
                ++d;
            }
            new_digits[i] = d;
        }

        free(digits);
        digits = new_digits;
        digits_len = (size_t)target;
    }

    return (seq_t)digits[(size_t)n];
}
