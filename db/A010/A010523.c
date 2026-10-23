/*
 * A010523  Decimal expansion of square root of 71.
 * https://oeis.org/A010523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9320, u16 n <= 9320, u32 n <= 9320, u64 n <= 9320,
 *            u128 n <= 9320, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: digit‐by‐digit extraction of the decimal expansion of √71.
   Computes a(1)=8 and a(n) the n‑th decimal digit for n≥2. */
#include <stdint.h>
#include <string.h>

#define BASE 1000000000U      /* 1e9, safe for multiplication by 100 */
#define MAX_WORDS 2000       /* enough for ~18000 decimal digits */

/* Simple arbitrary‑precision integer using base 10^9. Only operations
   required by the digit extraction algorithm are implemented. */
typedef struct {
    int len;              /* number of used words (least significant first) */
    uint32_t d[MAX_WORDS];
} BigInt;

/* initialise to zero */
static void bi_zero(BigInt *x) {
    x->len = 0;
    memset(x->d, 0, sizeof(x->d));
}

/* initialise to small unsigned integer */
static void bi_from_uint(BigInt *x, uint32_t v) {
    if (v == 0) { bi_zero(x); return; }
    x->len = 1;
    x->d[0] = v;
}

/* copy a into b */
static void bi_copy(BigInt *b, const BigInt *a) {
    b->len = a->len;
    memcpy(b->d, a->d, a->len * sizeof(uint32_t));
}

/* compare a and b: return -1 if a<b, 0 if a==b, 1 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (int i = a->len - 1; i >= 0; --i) {
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

/* a += small v (0 ≤ v < BASE) */
static void bi_add_small(BigInt *a, uint32_t v) {
    uint64_t sum = (uint64_t)a->d[0] + v;
    a->d[0] = (uint32_t)(sum % BASE);
    uint32_t carry = (uint32_t)(sum / BASE);
    int i = 1;
    while (carry && i < a->len) {
        uint64_t s = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(s % BASE);
        carry = (uint32_t)(s / BASE);
        ++i;
    }
    if (carry) {
        a->d[i++] = carry;
        a->len = i;
    }
}

/* a *= small v (0 ≤ v < BASE) */
static void bi_mul_small(BigInt *a, uint32_t v) {
    if (v == 0) { bi_zero(a); return; }
    uint64_t carry = 0;
    for (int i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * v + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* b = a - b; assumes a ≥ b */
static void bi_sub(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (int i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)diff;
    }
    /* trim leading zeros */
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)8;          /* first digit */

    /* current root (P) and remainder (R) as big integers */
    BigInt p, r, c, t, temp, prod;
    bi_from_uint(&p, 8);                    /* P = 8 */
    bi_from_uint(&r, 7);                    /* R = 71 - 8^2 = 7 */

    uint32_t digit = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* c = R * 100 */
        bi_copy(&c, &r);
        bi_mul_small(&c, 100);

        /* t = P * 20 */
        bi_copy(&t, &p);
        bi_mul_small(&t, 20);

        /* find largest d such that (t + d) * d <= c */
        for (uint32_t d = 9; ; --d) {
            bi_copy(&temp, &t);           /* temp = t */
            bi_add_small(&temp, d);       /* temp += d */
            bi_copy(&prod, &temp);        /* prod = temp */
            bi_mul_small(&prod, d);       /* prod *= d */

            if (bi_cmp(&prod, &c) <= 0) {
                digit = d;
                break;
            }
            if (d == 0) break;            /* should never happen */
        }

        /* update P = P * 10 + digit */
        bi_mul_small(&p, 10);
        bi_add_small(&p, digit);

        /* update R = c - prod */
        bi_copy(&r, &c);
        bi_sub(&r, &prod);

        if (i == n) return (seq_t)digit;
    }
    return (seq_t)0;   /* unreachable, but keeps the compiler happy */
}
