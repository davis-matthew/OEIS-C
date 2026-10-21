/*
 * A234930  Decimal expansion of sqrt(9.87654321).
 * https://oeis.org/A234930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function returns the n‑th decimal digit (a(1)=first digit) of
    sqrt(9.87654321).  The digit‑by‑digit extraction algorithm is used.
    It works with any seq_t (any unsigned integer type or arbitrary‑
    precision integer class) and any idx_t (uint64_t). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

/* ---------- tiny arbitrary‑precision integer (base 1e9) ---------- */
#define BI_BASE 1000000000U

typedef struct {
    uint32_t *d;    /* little‑endian limbs, d[0] is least significant   */
    size_t   len;   /* number of used limbs, 0 means zero                 */
} BigInt;

static void bi_init(BigInt *a)          { a->d = NULL; a->len = 0; }
static void bi_free(BigInt *a)          { free(a->d); a->d = NULL; a->len = 0; }
static void bi_norm(BigInt *a) {
    while (a->len && a->d[a->len-1] == 0) a->len--;
    if (a->len == 0) bi_free(a);
}
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->len = src->len;
    if (src->len) {
        dst->d = (uint32_t*)malloc(src->len * sizeof(uint32_t));
        memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    } else
        dst->d = NULL;
}
static void bi_mul_small(BigInt *a, uint32_t m) {
    if (m == 0) { bi_free(a); return; }
    if (m == 1) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BI_BASE);
        carry   = prod / BI_BASE;
    }
    while (carry) {
        a->d = (uint32_t*)realloc(a->d, (a->len + 1) * sizeof(uint32_t));
        a->d[a->len++] = (uint32_t)carry;
        carry /= BI_BASE;
    }
}
static void bi_add_small(BigInt *a, uint32_t inc) {
    uint64_t carry = inc;
    size_t i = 0;
    while (carry) {
        if (i >= a->len) {
            a->d = (uint32_t*)realloc(a->d, (a->len + 1) * sizeof(uint32_t));
            a->d[a->len++] = 0;
        }
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BI_BASE);
        carry   = sum / BI_BASE;
        ++i;
    }
}
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}
static void bi_sub(BigInt *a, const BigInt *b) { /* a -= b, assume a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (sub < 0) {
            sub += BI_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    bi_norm(a);
}

/* ------------------ main sequence function --------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* not used, but safe */

    /* Groups of 2 decimal digits of 987654321: 9, 87, 65, 43, 21 */
    const uint32_t groups[5] = {9U, 87U, 65U, 43U, 21U};

    BigInt rem, root, p, temp;
    bi_init(&rem);
    bi_init(&root);

    for (idx_t i = 1; i <= n; ++i) {
        /* bring down next two‐digit group */
        bi_mul_small(&rem, 100);
        uint32_t g = (i <= 5) ? groups[i-1] : 0U;
        bi_add_small(&rem, g);

        /* p = 20 * root */
        bi_copy(&p, &root);
        bi_mul_small(&p, 20);

        /* find largest digit d with (p + d)*d <= rem */
        int d = 0;
        for (int dd = 9; dd >= 0; --dd) {
            bi_copy(&temp, &p);
            bi_add_small(&temp, (uint32_t)dd);
            bi_mul_small(&temp, (uint32_t)dd);
            if (bi_cmp(&temp, &rem) <= 0) {
                d = dd;
                break;
            }
            bi_free(&temp);      /* free to reuse for next dd */
        }

        /* update remainder and root */
        bi_sub(&rem, &temp);
        bi_mul_small(&root, 10);
        bi_add_small(&root, (uint32_t)d);

        if (i == n) {
            /* clean up and return digit */
            bi_free(&rem);
            bi_free(&root);
            bi_free(&p);
            bi_free(&temp);
            return (seq_t)d;
        }

        bi_free(&temp);
        bi_free(&p);
    }

    /* should never reach here */
    bi_free(&rem);
    bi_free(&root);
    return 0;
}
