/*
 * A108026  Number of letters in the words formed by the digital recitation of
 *          sqrt(2) in the English language.
 * https://oeis.org/A108026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   a(n) = number of letters in the English word of the nth decimal
 *            digit of √2 (the first term is the integer part 1)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define BASE 1000000000U
/* Maximum digits needed for n up to about 10000:
   sqrt(2)*10^(2n) has 2n+1 decimal digits, each limb holds 9 decimal digits.
   For n = 10000 we need 2223 limbs.  Allocated 2500 for safety. */
#define MAX_LIMBS 2500U

typedef struct {
    uint32_t d[MAX_LIMBS];
    size_t len;
} BigInt;

static inline void bi_init(BigInt *x, uint64_t v) {
    x->len = 0;
    if (v == 0) return;
    while (v) {
        x->d[x->len++] = (uint32_t)(v % BASE);
        v /= BASE;
    }
}

static inline void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

static inline void bi_mul_small(BigInt *x, uint32_t m) {
    if (m == 0) { x->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t cur = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(cur % BASE);
        carry   = cur / BASE;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static inline void bi_add_small(BigInt *x, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < x->len) {
        uint64_t cur = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(cur % BASE);
        carry   = cur / BASE;
        ++i;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

static inline int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

static inline void bi_sub(BigInt *a, const BigInt *b) { /* a = a - b, assume a>=b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t cur = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (cur < 0) { cur += BASE; borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)cur;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

/* English word lengths for digits 0-9 */
static const int wordlen[10] = {4,3,3,5,4,4,3,5,5,4};

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset 1; undefined below, but safe */
    if (n == 1) return (seq_t)wordlen[1]; /* integer part '1' -> "one" */

    idx_t k = n - 1; /* number of digits after decimal needed */

    BigInt p, rem, base, tmp, cand;
    bi_init(&p, 1);          /* root so far */
    bi_init(&rem, 1);        /* remainder */

    int digit = 0;
    for (idx_t step = 0; step < k; ++step) {
        bi_mul_small(&rem, 100);             /* bring down two zeros */

        bi_copy(&base, &p);
        bi_mul_small(&base, 20);             /* 20 * p */

        for (int d = 9; d >= 0; --d) {
            bi_copy(&tmp, &base);
            bi_add_small(&tmp, (uint32_t)d);
            bi_copy(&cand, &tmp);
            bi_mul_small(&cand, (uint32_t)d);
            if (bi_cmp(&cand, &rem) <= 0) {
                digit = d;
                bi_sub(&rem, &cand);
                break;
            }
        }

        bi_mul_small(&p, 10);    /* append digit to the root */
        bi_add_small(&p, (uint32_t)digit);
    }

    return (seq_t)wordlen[digit];
}
