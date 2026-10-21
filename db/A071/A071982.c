/*
 * A071982  Parity of the decimal digits of sqrt(2).
 * https://oeis.org/A071982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Parity of decimal digits of sqrt(2): a(n) is 0 if the n‑th decimal digit of sqrt(2)
   (counting the integer part as digit 0) is even, 1 otherwise.  The
   implementation uses an exact digit‑by‑digit algorithm with a small
   base‑2^32 arbitrary‑precision integer, so it works for any seq_t
   (including 64‑bit and big‑integer types). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Big‑integer type:  base 2^32, little‑endian limbs.  The arrays are
   statically large enough for about 10 000 decimal digits (≈1100 limbs). */
#define BI_MAXLIMBS 4096
typedef struct {
    uint32_t d[BI_MAXLIMBS];
    size_t  len;    /* number of used limbs (little‑endian) */
} BigInt;

static void bi_set_uint(BigInt *a, uint32_t v) {
    a->len = 1;
    a->d[0] = v;
}
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i > 0; --i) {
        if (a->d[i-1] != b->d[i-1]) return (a->d[i-1] < b->d[i-1]) ? -1 : 1;
    }
    return 0;
}
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)prod;
        carry   = prod >> 32;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)carry;
        carry >>= 32;
    }
}
static void bi_add_small(BigInt *a, uint32_t m) {
    uint64_t sum = (uint64_t)a->d[0] + m;
    a->d[0] = (uint32_t)sum;
    uint64_t carry = sum >> 32;
    size_t i = 1;
    while (carry && i < a->len) {
        sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)sum;
        carry = sum >> 32;
        ++i;
    }
    if (carry) a->d[a->len++] = (uint32_t)carry;
}
static void bi_sub(BigInt *a, const BigInt *b) {        /* a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (diff < 0) {
            diff += ((int64_t)1 << 32);
            borrow = 1;
        } else borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (a->len && a->d[a->len-1] == 0) --a->len;
}

/* Core sequence function.  A static cache holds already computed digit
   parities, and static BigInt instances store the current state of the
   digit‑by‑digit sqrt extraction algorithm. */
seq_t a(idx_t n) {
    /* ---------- cache of digit parities ---------- */
    static seq_t *parity = NULL;
    static size_t parity_cap = 0;
    static size_t parity_len = 0;

    /* ---------- big‑integer state ---------- */
    static bool initialized = false;
    static BigInt root, rem, tmp, prod;   /* root: √2 digits so far, rem: remainder */

    if (!initialized) {
        /* initial token: integer part 1 (odd) */
        parity_cap = 16;
        parity = (seq_t *)malloc(sizeof(seq_t) * parity_cap);
        if (parity == NULL) exit(1);
        parity[0] = 1;
        parity_len = 1;

        bi_set_uint(&root, 1);
        bi_set_uint(&rem,  1);    /* 2 - 1² = 1 */
        initialized = true;
    }

    /* grow parity array if needed */
    while (parity_len <= (size_t)n) {
        if (parity_len >= parity_cap) {
            size_t new_cap = parity_cap << 1;
            seq_t *tmpp = (seq_t *)realloc(parity, new_cap * sizeof(seq_t));
            if (tmpp == NULL) exit(1);
            parity = tmpp;
            parity_cap = new_cap;
        }

        /* bring down next two zeros: remainder *= 100 */
        bi_mul_small(&rem, 100);

        /* candidate = 20 * root */
        bi_copy(&tmp, &root);
        bi_mul_small(&tmp, 20);

        /* find largest digit d (0..9) with (candidate + d) * d <= remainder */
        int chosen = 0;
        for (int d = 9; d >= 0; --d) {
            bi_copy(&prod, &tmp);
            bi_add_small(&prod, (uint32_t)d);   /* candidate + d */
            bi_mul_small(&prod, (uint32_t)d);   /* (candidate + d) * d */

            if (bi_cmp(&prod, &rem) <= 0) {
                chosen = d;
                bi_sub(&rem, &prod);                /* remainder -= product */
                bi_mul_small(&root, 10);
                bi_add_small(&root, (uint32_t)d);    /* root = root*10 + d */
                break;
            }
        }

        /* store parity of the chosen digit */
        parity[parity_len++] = (seq_t)(chosen & 1);
    }

    return parity[(size_t)n];
}
