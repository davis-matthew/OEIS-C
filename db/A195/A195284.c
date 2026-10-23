/*
 * A195284  Decimal expansion of shortest length of segment from side AB
 *          through incenter to side AC in right triangle ABC with sidelengths
 *          (a,b,c)=(3,4,5); i.e., decimal expansion of 2*sqrt(10)/3.
 * https://oeis.org/A195284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A195284: nth decimal digit of 2*sqrt(10)/3  */
/*  Implementation uses a manual big‑integer in base 1e9   */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BASE        1000000000U
#define MAX_LIMBS   2000          /* enough for 10^10000 */

/* big integer stored in little‑endian limbs (least significant first) */
typedef struct {
    uint32_t d[MAX_LIMBS];
    size_t   len;
} BigInt;

/* set to zero */
static void bi_zero(BigInt *a) {
    a->len = 0;
}

/* initialize from an unsigned 64 bit value */
static void bi_from_uint(BigInt *a, uint64_t v) {
    a->len = 0;
    while (v) {
        a->d[a->len++] = v % BASE;
        v /= BASE;
    }
}

/* normalize (remove leading zero limbs) */
static void bi_normalize(BigInt *a) {
    while (a->len && a->d[a->len-1] == 0) a->len--;
}

/* copy a big integer */
static void bi_copy(const BigInt *src, BigInt *dst) {
    dst->len = src->len;
    memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* compare a and b:  -1 if a<b, 0 if a==b, 1 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* add small integer (0..999) to big integer */
static void bi_add_small(BigInt *a, uint32_t v) {
    uint64_t carry = v;
    size_t i = 0;
    while (carry) {
        if (i >= a->len) {
            a->d[a->len++] = 0;
        }
        uint64_t cur = (uint64_t)a->d[i] + carry;
        a->d[i] = cur % BASE;
        carry   = cur / BASE;
        ++i;
    }
}

/* multiply big integer by small integer (0..9999) */
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t cur = (uint64_t)a->d[i] * m + carry;
        a->d[i] = cur % BASE;
        carry   = cur / BASE;
    }
    while (carry) {
        a->d[a->len++] = carry % BASE;
        carry /= BASE;
    }
}

/* subtract b from a: a = a - b; assumes a >= b */
static void bi_sub_self(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t ai = a->d[i];
        int64_t bi = (i < b->len) ? b->d[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)diff;
    }
    bi_normalize(a);
}

/* divide big integer a by small divisor d (<= 10); return quotient q */
static void bi_div_small(const BigInt *a, uint32_t d, BigInt *q) {
    q->len = a->len;
    uint64_t rem = 0;
    for (size_t i = a->len; i-- > 0;) {
        uint64_t cur = a->d[i] + rem * BASE;
        q->d[i] = cur / d;
        rem = cur % d;
    }
    bi_normalize(q);
}

/* power10: compute 10^exp, exp >= 0 */
static void bi_power10(uint32_t exp, BigInt *out) {
    uint32_t pow10_small[10] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
    uint32_t g = exp / 9;
    uint32_t rem = exp % 9;
    out->len = g + 1;
    for (size_t i = 0; i < out->len; ++i) out->d[i] = 0;
    out->d[g] = pow10_small[rem];
    bi_normalize(out);
}

/* convert big integer to decimal string; buffer must be big enough */
static void bi_to_decimal(const BigInt *a, char *buf) {
    if (a->len == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    int pos = 0;
    /* most significant limb */
    pos += sprintf(buf + pos, "%u", a->d[a->len - 1]);
    /* remaining limbs padded with leading zeros */
    for (size_t i = a->len - 1; i-- > 0;) {
        pos += sprintf(buf + pos, "%09u", a->d[i]);
    }
    buf[pos] = '\0';
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;          /* first digit is 2 */

    /* compute up to some extra digits to guarantee accuracy */
    uint32_t K = (uint32_t)(n - 1);        /* position after decimal point (1‑based) */
    uint32_t M = K + 10;                   /* extra 10 digits margin */

    /* big integers needed */
    BigInt result, remainder, cand, temp, pow10M, Y, Q;
    bi_zero(&result);
    bi_zero(&remainder);

    /* digit‑by‑digit sqrt of 10^(2M+1) */
    for (int i = M; i >= 0; --i) {
        bi_mul_small(&remainder, 100);
        int pair = (i == M) ? 10 : 0;
        bi_add_small(&remainder, (uint32_t)pair);

        /* candidate = result * 20 */
        bi_copy(&result, &cand);
        bi_mul_small(&cand, 20);

        /* find largest d ∈ [0,9] with (cand + d) * d <= remainder */
        int d;
        for (d = 9; d >= 0; --d) {
            bi_copy(&cand, &temp);
            bi_add_small(&temp, (uint32_t)d);
            bi_mul_small(&temp, (uint32_t)d);
            if (bi_cmp(&temp, &remainder) <= 0) break;
        }

        /* remainder -= temp */
        bi_sub_self(&remainder, &temp);

        /* result = result * 10 + d */
        bi_mul_small(&result, 10);
        bi_add_small(&result, (uint32_t)d);
    }

    /* result now holds floor(sqrt10 * 10^M) */
    /* compute Z = result - 3 * 10^M (fractional part times 10^M) */
    bi_power10(M, &pow10M);          /* 10^M */
    bi_mul_small(&pow10M, 3);        /* 3 * 10^M */
    bi_sub_self(&result, &pow10M);   /* result becomes Z */
    bi_copy(&result, &Y);            /* Y = Z * 2 */
    bi_mul_small(&Y, 2);

    /* Q = floor(Y / 3) */
    bi_div_small(&Y, 3, &Q);

    /* convert Q to decimal string */
    char dec[MAX_LIMBS*10];          /* enough for all digits */
    bi_to_decimal(&Q, dec);
    size_t lenQ = strlen(dec);
    /* pad with leading zeros to length M if necessary */
    if (lenQ < M) {
        size_t pad = M - lenQ;
        memmove(dec + pad, dec, lenQ + 1);
        memset(dec, '0', pad);
    }

    /* K is 1‑based position after decimal; K-1 is 0‑based index in string */
    char digit_char = dec[K - 1];
    return (seq_t)(digit_char - '0');
}
