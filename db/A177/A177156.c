/*
 * A177156  Decimal expansion of (9+sqrt(221))/14.
 * https://oeis.org/A177156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = floor(((9+sqrt(221))*10^(n-1))/14) % 10
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) is the first digit of (9+sqrt(221))/14 (the integer part). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/* --- Tiny big‑integer helpers (base 1e9) --- */
typedef struct {
    size_t n;          /* number of used limbs */
    size_t cap;        /* allocated limbs       */
    uint32_t *d;       /* little‑endian limbs   */
} bigint;

static void bi_zero(bigint *b) {
    b->n = 0;
    b->cap = 4;
    b->d = (uint32_t *)calloc(b->cap, sizeof(uint32_t));
    assert(b->d);
}

static void bi_ensure(bigint *b, size_t min_cap) {
    if (b->cap < min_cap) {
        size_t new_cap = b->cap ? b->cap * 2 : 4;
        while (new_cap < min_cap) new_cap *= 2;
        b->d = (uint32_t *)realloc(b->d, new_cap * sizeof(uint32_t));
        assert(b->d);
        b->cap = new_cap;
    }
}

static void bi_trim(bigint *b) {
    while (b->n && b->d[b->n-1] == 0) b->n--;
}

static void bi_set_u64(bigint *b, uint64_t v) {
    bi_zero(b);
    while (v) {
        bi_ensure(b, b->n+1);
        b->d[b->n++] = (uint32_t)(v % 1000000000U);
        v /= 1000000000U;
    }
}

static void bi_mul_small(bigint *b, uint32_t m) {
    if (m == 0 || b->n == 0) { b->n = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->n; i++) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % 1000000000U);
        carry = prod / 1000000000U;
    }
    if (carry) {
        bi_ensure(b, b->n+1);
        b->d[b->n++] = (uint32_t)carry;
    }
}

static void bi_add_small(bigint *b, uint32_t m) {
    if (m == 0) return;
    uint64_t carry = m;
    size_t i = 0;
    while (carry && i < b->n) {
        uint64_t sum = (uint64_t)b->d[i] + carry;
        b->d[i] = (uint32_t)(sum % 1000000000U);
        carry = sum / 1000000000U;
        i++;
    }
    while (carry) {
        bi_ensure(b, b->n+1);
        b->d[b->n++] = (uint32_t)carry;
        carry /= 1000000000U;
    }
}

static int bi_cmp(const bigint *a, const bigint *b) {
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (size_t i = a->n; i-- > 0;) {
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

static void bi_sub_inplace(bigint *a, const bigint *b) {
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->n; i++) {
        uint64_t ai = a->d[i];
        uint64_t bi = (i < b->n) ? b->d[i] : 0;
        uint64_t diff = ai - bi - borrow;
        if (diff >> 63) {          /* negative */
            a->d[i] = (uint32_t)(diff + 1000000000U);
            borrow = 1;
        } else {
            a->d[i] = (uint32_t)diff;
            borrow = 0;
        }
    }
    assert(borrow == 0);
    bi_trim(a);
}

static void bi_copy(bigint *dest, const bigint *src) {
    bi_ensure(dest, src->n);
    memcpy(dest->d, src->d, src->n * sizeof(uint32_t));
    dest->n = src->n;
}

static void bi_free(bigint *b) {
    free(b->d);
    b->d = NULL;
    b->n = b->cap = 0;
}

/* --- modular exponentiation: (10^k) mod mod --- */
static unsigned int mod_pow10(unsigned long long k, unsigned int mod) {
    unsigned int base = 10 % mod;
    unsigned int res = 1 % mod;
    while (k) {
        if (k & 1ULL) res = (unsigned int)((unsigned long long)res * base % mod);
        base = (unsigned int)((unsigned long long)base * base % mod);
        k >>= 1;
    }
    return res;
}

/* --- compute sqrt(221)*10^k floor modulo 140 --- */
static unsigned int sqrt221_mod140_of(unsigned long long k) {
    const unsigned int MOD = 140U;
    bigint rem, cur, tmp1, tmp2, prod;
    bi_set_u64(&rem, 25ULL);          /* remainder after integer part */
    bi_set_u64(&cur, 14ULL);          /* integer part of sqrt(221) */
    unsigned int cur_mod = 14U % MOD; /* cur modulo 140 */

    bi_zero(&tmp1);
    bi_zero(&tmp2);
    bi_zero(&prod);

    for (unsigned long long i = 0; i < k; ++i) {
        /* remainder = remainder * 100 */
        bi_mul_small(&rem, 100U);

        /* tmp1 = 20 * cur */
        bi_copy(&tmp1, &cur);
        bi_mul_small(&tmp1, 20U);

        /* find largest digit d (0..9) such that (tmp1 + d) * d <= rem */
        unsigned int best = 0;
        for (int d = 9; d >= 0; --d) {
            bi_copy(&tmp2, &tmp1);
            bi_add_small(&tmp2, (uint32_t)d);      /* tmp2 = 20*cur + d */
            bi_copy(&prod, &tmp2);
            bi_mul_small(&prod, (uint32_t)d);      /* prod = (20*cur + d) * d */

            if (bi_cmp(&prod, &rem) <= 0) {
                best = (unsigned int)d;
                bi_sub_inplace(&rem, &prod);
                break;
            }
        }

        /* cur = cur*10 + best */
        bi_mul_small(&cur, 10U);
        bi_add_small(&cur, best);

        cur_mod = (cur_mod * 10U + best) % MOD;
    }

    bi_free(&rem);
    bi_free(&cur);
    bi_free(&tmp1);
    bi_free(&tmp2);
    bi_free(&prod);
    return cur_mod;
}

/* --- public function required by the build --- */
seq_t a(idx_t n)
{
    /* n >= 1; a(1) is the first digit (the integer part) */
    unsigned long long k = n - 1ULL;           /* shift to 0‑based exponent */

    /* compute 10^k mod 140 */
    unsigned int pow10_mod = mod_pow10(k, 140U);

    /* compute sqrt(221)*10^k mod 140 */
    unsigned int sqrt_mod = sqrt221_mod140_of(k);

    /* numerator modulo 140 */
    unsigned int num_mod = ((9U % 140U) * pow10_mod + sqrt_mod) % 140U;

    /* a(n) = floor(num_mod/14) (0 <= num_mod < 140) */
    unsigned int digit = num_mod / 14U;
    return (seq_t)digit;
}
