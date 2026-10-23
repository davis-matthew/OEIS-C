/*
 * A020043  a(n) = round(Gamma(n+2/3)/Gamma(2/3)).
 * https://oeis.org/A020043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(Gamma(n+2/3)/Gamma(2/3))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed as round(P(n)/3^n) where P(n)=∏_{k=0}^{n-1}(3k+2). */
/* Uses an arbitrary-precision integer (internal BigUint) to avoid overflow. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t *digits; /* little‑endian, each limb holds 32 bits */
    size_t    len;    /* number of limbs */
} BigUint;

static BigUint *biguint_new(size_t init_len)
{
    BigUint *x = (BigUint *)malloc(sizeof(BigUint));
    x->len = init_len;
    x->digits = init_len ? (uint32_t *)calloc(init_len, sizeof(uint32_t)) : NULL;
    return x;
}
static void biguint_free(BigUint *x)
{
    free(x->digits);
    free(x);
}
static void biguint_trim(BigUint *x)
{
    while (x->len && x->digits[x->len - 1] == 0) {
        --x->len;
    }
}
static void biguint_mul_small(BigUint *x, uint32_t m)
{
    if (m == 1) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->digits[i] * m + carry;
        x->digits[i] = (uint32_t)(prod & 0xFFFFFFFFu);
        carry = prod >> 32;
    }
    if (carry) {
        if (x->len == 0) {
            x->digits = (uint32_t *)malloc(sizeof(uint32_t));
            x->len = 1;
            x->digits[0] = (uint32_t)carry;
        } else {
            x->digits = (uint32_t *)realloc(x->digits, (x->len + 1) * sizeof(uint32_t));
            x->digits[x->len++] = (uint32_t)carry;
        }
    }
}
static void biguint_add_small(BigUint *x, uint32_t v)
{
    uint64_t carry = v;
    size_t i = 0;
    while (carry) {
        if (i == x->len) {
            x->digits = (uint32_t *)realloc(x->digits,
                                            (x->len + 1) * sizeof(uint32_t));
            x->digits[x->len++] = 0;
        }
        uint64_t sum = (uint64_t)x->digits[i] + carry;
        x->digits[i] = (uint32_t)(sum & 0xFFFFFFFFu);
        carry = sum >> 32;
        ++i;
    }
}
static void biguint_sub_small(BigUint *x, uint32_t v) /* assumes x >= v */
{
    int64_t borrow = v;
    size_t i = 0;
    while (borrow) {
        int64_t diff = (int64_t)x->digits[i] - borrow;
        if (diff < 0) {
            x->digits[i] = (uint32_t)(diff + ((int64_t)1 << 32));
            borrow = 1;
        } else {
            x->digits[i] = (uint32_t)diff;
            borrow = 0;
        }
        ++i;
    }
    biguint_trim(x);
}
static void biguint_div_small(BigUint *x, uint32_t d, uint32_t *rem)
{
    uint64_t r = 0;
    for (int i = (int)x->len - 1; i >= 0; --i) {
        uint64_t cur = (r << 32) + x->digits[i];
        x->digits[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    *rem = (uint32_t)r;
    biguint_trim(x);
}
static void biguint_add(BigUint *x, const BigUint *y)
{
    size_t maxlen = x->len > y->len ? x->len : y->len;
    if (x->len < maxlen) {
        x->digits = (uint32_t *)realloc(x->digits,
                                        maxlen * sizeof(uint32_t));
        for (size_t i = x->len; i < maxlen; ++i) x->digits[i] = 0;
        x->len = maxlen;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < maxlen; ++i) {
        uint64_t sum = carry + x->digits[i];
        if (i < y->len) sum += y->digits[i];
        x->digits[i] = (uint32_t)(sum & 0xFFFFFFFFu);
        carry = sum >> 32;
    }
    if (carry) {
        x->digits = (uint32_t *)realloc(x->digits,
                                        (x->len + 1) * sizeof(uint32_t));
        x->digits[x->len++] = (uint32_t)carry;
    }
}

static seq_t bigint_to_seq(const BigUint *x)
{
    seq_t res = 0;
    const seq_t BASE = (seq_t)4294967296ULL; /* 2^32 */
    for (int i = (int)x->len - 1; i >= 0; --i) {
        res = (res * BASE) + (seq_t)x->digits[i];
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Special case n=0 */
    if (n == 0) return (seq_t)1;

    /* Compute product of (3k+2) and 3^n  */
    BigUint *P = biguint_new(1);
    P->len = 1;
    P->digits[0] = 1;
    BigUint *D = biguint_new(1);
    D->len = 1;
    D->digits[0] = 1;

    for (idx_t k = 0; k < n; ++k) {
        uint32_t fact = 3 * (uint32_t)k + 2;
        biguint_mul_small(P, fact);
        biguint_mul_small(D, 3);
    }

    /* M = (D - 1) / 2  */
    BigUint *M = biguint_new(D->len);
    M->len = D->len;
    M->digits = (uint32_t *)malloc(D->len * sizeof(uint32_t));
    for (size_t i = 0; i < D->len; ++i) M->digits[i] = D->digits[i];
    biguint_sub_small(M, 1);
    uint32_t dummy;
    biguint_div_small(M, 2, &dummy); /* dummy is 0 */

    /* Q = P + M */
    BigUint *Q = biguint_new(P->len);
    Q->len = P->len;
    Q->digits = (uint32_t *)malloc(P->len * sizeof(uint32_t));
    for (size_t i = 0; i < P->len; ++i) Q->digits[i] = P->digits[i];
    biguint_add(Q, M);

    /* Divide Q by 3 n times (i.e., by D = 3^n) */
    for (idx_t i = 0; i < n; ++i)
        biguint_div_small(Q, 3, &dummy); /* dummy holds remainder */

    /* Convert to seq_t */
    seq_t result = bigint_to_seq(Q);

    /* Free memory */
    biguint_free(P);
    biguint_free(D);
    biguint_free(M);
    biguint_free(Q);

    return result;
}
