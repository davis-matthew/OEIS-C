/*
 * A100125  Decimal expansion of Sum_{n>0} n/(2^(n^2)).
 * https://oeis.org/A100125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of the series Σ_{k>0} k / 2^(k^2)  */
#include <stdint.h>
#include <math.h>

/* Maximum number of 64‑bit words needed for n up to 10000.
   2 * (n*log2(10)+100) bits ≈ 66 600 bits ≈ 1 050 words.                      */
#define MAX_WORDS 2000

typedef struct {
    uint64_t w[MAX_WORDS];
    int len;                     /* number of used words (least significant first) */
} BigInt;

/* ---------- basic big‑integer helpers ---------- */
static inline void bi_zero(BigInt *a) { a->len = 0; }

static inline void bi_set_uint(BigInt *a, uint64_t v) {
    if (v == 0) { a->len = 0; return; }
    a->w[0] = v; a->len = 1;
}

static inline void bi_set_power_two(BigInt *a, unsigned shift) {
    unsigned idx = shift >> 6;          /* shift / 64 */
    unsigned bit = shift & 63;          /* shift % 64 */
    a->len = idx + 1;
    for (int i = 0; i < a->len; ++i) a->w[i] = 0;
    if (bit == 0) a->w[idx] = 1ULL;
    else          a->w[idx] = 1ULL << bit;
}

/* multiply a by small integer m, in-place (res = a * m) */
static inline void bi_mul_uint_inplace(BigInt *a, uint64_t m) {
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (int i = 0; i < a->len; ++i) {
        __uint128_t prod = (__uint128_t)a->w[i] * m + carry;
        a->w[i] = (uint64_t)prod;
        carry   = (uint64_t)(prod >> 64);
    }
    if (carry) {
        a->w[a->len] = carry;
        a->len++;
    }
}

/* res = a + b  (big‑integer addition) */
static inline void bi_add(BigInt *res, const BigInt *a, const BigInt *b) {
    int maxlen = a->len > b->len ? a->len : b->len;
    uint64_t carry = 0;
    for (int i = 0; i < maxlen; ++i) {
        __uint128_t sum = carry;
        if (i < a->len) sum += a->w[i];
        if (i < b->len) sum += b->w[i];
        res->w[i] = (uint64_t)sum;
        carry     = (uint64_t)(sum >> 64);
    }
    if (carry) {
        res->w[maxlen] = carry;
        res->len = maxlen + 1;
    } else
        res->len = maxlen;
}

/* res = a plus small integer v (in-place)  */
static inline void bi_add_uint_inplace(BigInt *a, uint64_t v) {
    uint64_t carry = v;
    for (int i = 0; i < a->len && carry; ++i) {
        __uint128_t sum = (__uint128_t)a->w[i] + carry;
        a->w[i] = (uint64_t)sum;
        carry   = (uint64_t)(sum >> 64);
    }
    if (carry) {
        a->w[a->len] = carry;
        a->len++;
    }
}

/* res = a >> shift (shift bits to the right)  */
static inline void bi_shrink_right_inplace(BigInt *a, unsigned shift) {
    if (shift == 0 || a->len == 0) return;
    unsigned word_shift = shift >> 6;
    unsigned bit_shift  = shift & 63;
    if (word_shift >= (unsigned)a->len) {
        a->len = 0;
        return;
    }
    int newlen = a->len - word_shift;
    for (int i = 0; i < newlen; ++i) {
        uint64_t val = a->w[i + word_shift] >> bit_shift;
        if (bit_shift && i + word_shift + 1 < a->len)
            val |= a->w[i + word_shift + 1] << (64 - bit_shift);
        a->w[i] = val;
    }
    a->len = newlen;
}

/* remainder of a modulo m (m fits in 64 bits) */
static inline uint64_t bi_mod_uint(const BigInt *a, uint64_t m) {
    uint64_t rem = 0;
    for (int i = a->len - 1; i >= 0; --i) {
        __uint128_t acc = ((__uint128_t)rem << 64) + a->w[i];
        rem = (uint64_t)(acc % m);
    }
    return rem;
}

/* copy big integer (a into b) */
static inline void bi_copy(BigInt *dest, const BigInt *src) {
    dest->len = src->len;
    for (int i = 0; i < src->len; ++i) dest->w[i] = src->w[i];
}

/* ---------- the sequence function ---------- */
seq_t a(idx_t n) {
    static const double LOG2_10 = 3.321928094887362347870319429;  /* log2(10) */

    /* large enough bit width to capture n‑th decimal digit accurately   */
    unsigned X = (unsigned)ceil(((double)(n + 1)) * LOG2_10) + 100U;  /* bits */
    unsigned kmax = (unsigned)sqrt((double)X);

    BigInt N; bi_zero(&N);

    /* accumulate terms k * 2^(X - k^2)  */
    for (unsigned k = 1; k <= kmax; ++k) {
        unsigned shift = X - k * k;
        BigInt term; bi_set_power_two(&term, shift);
        bi_mul_uint_inplace(&term, k);
        bi_add(&N, &N, &term);
    }

    /* multiply by 10^(n+1) */
    BigInt A; bi_copy(&A, &N);
    for (unsigned i = 0; i <= n; ++i)
        bi_mul_uint_inplace(&A, 10);

    /* divide (shift right) by 2^X  */
    bi_shrink_right_inplace(&A, X);

    /* the digit is the least significant decimal digit of A  */
    uint64_t dig = bi_mod_uint(&A, 10u);

    return (seq_t)dig;
}
