/*
 * A175542  Smallest integer t such that 1 + binomial(n,1) + binomial(n,2) +
 *          ... + binomial(n,t) is a power of 2.
 * https://oeis.org/A175542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175542: smallest t such that
 * 1 + C(n,1) + C(n,2) + ... + C(n,t) is a power of two. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --- big integer implementation (limb0 = least significant 64 bits) --- */
#define BIG_LIMBS 256

typedef struct {
    uint64_t limb[BIG_LIMBS];
    size_t   len;      /* number of used limbs (0 for zero) */
} big_uint;

/* initialise to zero */
static inline void big_init_zero(big_uint *a) {
    a->len = 0;
}

/* initialise to unsigned 64‑bit value */
static inline void big_set_uint64(big_uint *a, uint64_t v) {
    if (v == 0) {
        a->len = 0;
    } else {
        a->len = 1;
        a->limb[0] = v;
    }
}

/* copy a to b */
static inline void big_copy(big_uint *b, const big_uint *a) {
    b->len = a->len;
    for (size_t i = 0; i < a->len; ++i) b->limb[i] = a->limb[i];
}

/* add b to a (a += b) */
static inline void big_add(big_uint *a, const big_uint *b) {
    size_t max = a->len > b->len ? a->len : b->len;
    unsigned __int128 carry = 0;
    for (size_t i = 0; i < max; ++i) {
        unsigned __int128 ai = (i < a->len) ? a->limb[i] : 0;
        unsigned __int128 bi = (i < b->len) ? b->limb[i] : 0;
        unsigned __int128 sum = ai + bi + carry;
        if (i < BIG_LIMBS) a->limb[i] = (uint64_t)sum;
        carry = sum >> 64;
    }
    a->len = max;
    if (carry) {
        if (a->len < BIG_LIMBS) a->limb[a->len++] = (uint64_t)carry;
        else a->len = BIG_LIMBS; /* overflow ignored – value is still fine for our limit */
    }
}

/* multiply a by small unsigned 64‑bit value (a *= m) */
static inline void big_mul_small(big_uint *a, uint64_t m) {
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    unsigned __int128 carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        unsigned __int128 prod = (unsigned __int128)a->limb[i] * m + carry;
        a->limb[i] = (uint64_t)prod;
        carry = prod >> 64;
    }
    if (carry) {
        if (a->len < BIG_LIMBS) a->limb[a->len++] = (uint64_t)carry;
    }
}

/* divide a by small unsigned 64‑bit value (exact division) (a /= d) */
static inline void big_div_small(big_uint *a, uint64_t d) {
    if (d == 0 || a->len == 0) return;
    unsigned __int128 rem = 0;
    for (size_t i = a->len; i-- > 0; ) {
        unsigned __int128 cur = (rem << 64) | a->limb[i];
        uint64_t q = (uint64_t)(cur / d);
        rem = cur % d;
        a->limb[i] = q;
    }
    /* trim leading zeros */
    while (a->len && a->limb[a->len-1] == 0) a->len--;
}

/* test if a is a power of two (a>0 and exactly one bit set) */
static inline bool big_is_power_of_two(const big_uint *a) {
    if (a->len == 0) return false;
    /* only one limb may be non‑zero and that limb must be a power of two */
    bool found = false;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t v = a->limb[i];
        if (v == 0) continue;
        if (found || (v & (v - 1)) != 0) return false; /* more than one bit */
        found = true;
    }
    return found;
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    /* initialise binomial coefficient C(n,0)=1 and sum=1 */
    big_uint binom, sum;
    big_init_zero(&binom);
    big_set_uint64(&binom, 1);
    big_copy(&sum, &binom);

    /* iterate t = 1 .. n */
    for (idx_t t = 1; t <= n; ++t) {
        /* binom = binom * (n - t + 1) / t */
        uint64_t mult = (uint64_t)(n - t + 1);
        big_mul_small(&binom, mult);
        big_div_small(&binom, (uint64_t)t);

        /* sum += binom */
        big_add(&sum, &binom);

        /* test for power of two */
        if (big_is_power_of_two(&sum))
            return (seq_t)t;
    }

    /* should never reach here – fallback to n */
    return (seq_t)n;
}
