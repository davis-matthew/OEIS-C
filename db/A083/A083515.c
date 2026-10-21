/*
 * A083515  Smallest number k such that R(n) + k is a square, where R(n) =
 *          A002275.
 * https://oeis.org/A083515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(sqrt(R(n)))^2 - R(n), R(n) = 111…1 (n ones)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

/* ---- small big‑integer library used by the algorithm ----------------- */
#define BASE 1000000000U          /* 1e9, fits in 32 bits, product < 2^64 */
#define MAX_DIGITS 1300           /* holds up to ~ 12000 decimal digits */

typedef struct {
    uint32_t d[MAX_DIGITS];
    size_t len;                   /* number of used limbs, little endian */
} Big;

/* initialise to zero */
static void big_set_zero(Big *b) { b->len = 0; }

/* copy a into b */
static void big_copy(Big *b, const Big *a) {
    b->len = a->len;
    memcpy(b->d, a->d, a->len * sizeof(uint32_t));
}

/* remove leading zero limbs */
static void big_normalize(Big *b) {
    while (b->len && b->d[b->len - 1] == 0) b->len--;
}

/* add a small unsigned integer to big integer */
static void big_add_small(Big *b, uint32_t add) {
    uint64_t sum = (b->len ? b->d[0] : 0) + add;
    if (!b->len) b->len = 1;
    b->d[0] = (uint32_t)(sum % BASE);
    uint64_t carry = sum / BASE;
    size_t i = 1;
    while (carry && i < b->len) {
        sum = (uint64_t)b->d[i] + carry;
        b->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        i++;
    }
    if (carry) {
        b->d[b->len++] = (uint32_t)carry;
    }
}

/* multiply big integer by a small unsigned integer */
static void big_mul_small(Big *b, uint32_t m) {
    if (m == 0 || b->len == 0) { b->len = 0; return; }
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)(prod % BASE);
        carry    = prod / BASE;
    }
    while (carry) {
        assert(i < MAX_DIGITS);
        b->d[i++] = (uint32_t)(carry % BASE);
        carry     = carry / BASE;
    }
    b->len = i;
}

/* compare two big integers: return +1 if a>b, 0 if equal, -1 if a<b */
static int big_cmp(const Big *a, const Big *b) {
    if (a->len != b->len) return a->len > b->len ? 1 : -1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return a->d[i] > b->d[i] ? 1 : -1;
    }
    return 0;
}

/* subtract b from a (a>=b) and store in res */
static void big_sub(const Big *a, const Big *b, Big *res) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) { diff += BASE; borrow = 1; } else borrow = 0;
        res->d[i] = (uint32_t)diff;
    }
    res->len = a->len;
    big_normalize(res);
}

/* add two big integers and store in res */
static void big_add(const Big *a, const Big *b, Big *res) {
    uint64_t carry = 0;
    size_t len = a->len > b->len ? a->len : b->len;
    size_t i;
    for (i = 0; i < len; ++i) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        res->d[i] = (uint32_t)(sum % BASE);
        carry     = sum / BASE;
    }
    if (carry) {
        assert(i < MAX_DIGITS);
        res->d[i++] = (uint32_t)carry;
    }
    res->len = i;
}

/* check if big integer is zero */
static int big_is_zero(const Big *b) { return b->len == 0; }

/* ---- core sequence function ------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset 1: index 0 unused */

    /* big integer buffers */
    Big c, p, tmp1, tmp2;
    big_set_zero(&c);
    big_set_zero(&p);

    /* number of pairs of decimal digits in the repunit */
    size_t pairs = (n + 1) / 2;
    int first_pair_len = (n & 1) ? 1 : 2;   /* 1 if n odd, else 2 */

    for (size_t idx = 0; idx < pairs; ++idx) {
        uint32_t pair_val;
        if (idx == 0 && first_pair_len == 1) {
            pair_val = 1;                /* first (most significant) single digit */
        } else {
            pair_val = 11;                /* all subsequent pairs are 11 */
        }

        /* p = p*100 + pair_val */
        big_mul_small(&p, 100);
        big_add_small(&p, pair_val);

        /* find largest x in 0..9 such that (c*20 + x)*x <= p */
        uint32_t x;
        for (x = 9; ; --x) {
            big_copy(&tmp1, &c);
            big_mul_small(&tmp1, 20);          /* tmp1 = c*20 */
            big_add_small(&tmp1, x);           /* tmp1 = c*20 + x */
            big_mul_small(&tmp1, x);           /* tmp1 = (c*20 + x)*x */
            if (big_cmp(&tmp1, &p) <= 0) break;
        }

        /* p -= (c*20 + x)*x */
        big_sub(&p, &tmp1, &p);
        /* c = c*10 + x */
        big_mul_small(&c, 10);
        big_add_small(&c, x);
    }

    /* if exact square, answer is 0 */
    if (big_is_zero(&p)) return 0;

    /* diff = 2*c + 1 - p */
    big_copy(&tmp2, &c);                 /* tmp2 = c */
    big_mul_small(&tmp2, 2);             /* tmp2 = 2*c */
    big_add_small(&tmp2, 1);             /* tmp2 = 2*c + 1 */
    big_sub(&tmp2, &p, &tmp2);           /* tmp2 = 2*c + 1 - p */

    /* convert big integer tmp2 to seq_t (truncation on overflow) */
    seq_t result = 0;
    for (size_t i = 0; i < tmp2.len; ++i) {
        result = result * (seq_t)BASE + tmp2.d[tmp2.len - 1 - i];
    }
    return result;
}
