/*
 * A011548  Decimal expansion of sqrt(2) rounded to n places.
 * https://oeis.org/A011548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = round(sqrt(2) * 10^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the decimal expansion of √2 rounded to n places. */

#include <stdint.h>
#include <string.h>

typedef uint32_t limb_t;
#define LIMB_BASE 1000000000U  /* 1e9, fits in 32 bits */
#define MAX_LIMBS 3000        /* enough for n up to ~10000 */

/* Very small full‑precision integer implementation (base 1e9, little‑endian). */
typedef struct {
    int used;                /* number of used limbs */
    limb_t d[MAX_LIMBS];
} BigInt;

/* Initialise a BigInt to zero. */
static void bi_init(BigInt *a) {
    a->used = 0;
}

/* Assign from a small unsigned integer (non‑negative, <1e9). */
static void bi_assign_uint(BigInt *a, uint32_t v) {
    if (v == 0) {
        a->used = 0;
    } else {
        a->used = 1;
        a->d[0] = v;
    }
}

/* Copy one BigInt to another. */
static void bi_copy(BigInt *dst, const BigInt *src) {
    dst->used = src->used;
    memcpy(dst->d, src->d, src->used * sizeof(limb_t));
}

/* Trim leading zero limbs. */
static void bi_clean(BigInt *a) {
    while (a->used > 0 && a->d[a->used - 1] == 0)
        a->used--;
}

/* Compare two BigInts: return -1 if a<b, 0 if a==b, +1 if a>b. */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->used != b->used)
        return a->used < b->used ? -1 : 1;
    for (int i = a->used - 1; i >= 0; --i) {
        if (a->d[i] != b->d[i])
            return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

/* Multiply a BigInt by a small integer (0 ≤ v < LIMB_BASE). */
static void bi_mul_small(BigInt *a, uint32_t v) {
    if (v == 0 || a->used == 0) {
        a->used = 0;
        return;
    }
    uint64_t carry = 0;
    for (int i = 0; i < a->used; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * v + carry;
        a->d[i] = (limb_t)(prod % LIMB_BASE);
        carry   = prod / LIMB_BASE;
    }
    while (carry) {
        a->d[a->used++] = (limb_t)(carry % LIMB_BASE);
        carry /= LIMB_BASE;
    }
}

/* Add a small integer (0 ≤ v < LIMB_BASE) to a BigInt. */
static void bi_add_small(BigInt *a, uint32_t v) {
    uint64_t carry = v;
    int i = 0;
    while (carry && i < a->used) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (limb_t)(sum % LIMB_BASE);
        carry   = sum / LIMB_BASE;
        ++i;
    }
    if (carry) {
        a->d[a->used++] = (limb_t)carry;
    }
}

/* Subtract b from a (a >= b). */
static void bi_sub(BigInt *a, const BigInt *b) {
    int64_t borrow = 0;
    for (int i = 0; i < a->used; ++i) {
        int64_t sub = (int64_t)a->d[i] - borrow - (i < b->used ? b->d[i] : 0);
        if (sub < 0) {
            sub += LIMB_BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->d[i] = (limb_t)sub;
    }
    bi_clean(a);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Arrays for decimal digits of sqrt(2).  Index 0 holds the integer part. */
    int *digits = (int *)malloc((n + 2) * sizeof(int));
    if (!digits) return 0;  /* malloc failure; should not happen for small n */

    digits[0] = 1; /* integer part of √2 */

    BigInt p, r, div, temp, tempMul;
    bi_init(&p);
    bi_init(&r);
    bi_assign_uint(&p, 1);  /* current number (integer part + decimals so far) */
    bi_assign_uint(&r, 1);  /* remainder = 2 - 1^2 = 1 */

    for (idx_t i = 0; i <= n; ++i) {
        /* Bring down next two decimal zeros */
        bi_mul_small(&r, 100);

        /* divisor = 20 * p */
        bi_copy(&div, &p);
        bi_mul_small(&div, 20);

        /* Find largest x (0‑9) such that (div + x) * x <= r */
        unsigned int x;
        for (x = 9; x <= 9; --x) {  /* loop stops at x == 0 */
            bi_copy(&temp, &div);
            bi_add_small(&temp, x);
            bi_copy(&tempMul, &temp);
            bi_mul_small(&tempMul, x);
            if (bi_cmp(&tempMul, &r) <= 0) break;
            if (x == 0) break;  /* avoid unsigned wrap‑around */
        }

        /* Update remainder and current number */
        bi_sub(&r, &tempMul);
        bi_mul_small(&p, 10);
        bi_add_small(&p, x);
        digits[i + 1] = (int)x;
    }

    /* Rounding: look at the (n+1)‑th decimal digit */
    if (digits[n + 1] >= 5) {
        int pos = n;
        int carry = 1;
        while (pos >= 0 && carry) {
            int sum = digits[pos] + carry;
            digits[pos] = sum % 10;
            carry = sum / 10;
            --pos;
        }
        if (carry) {
            /* Wrap the digits array: insert a leading 1 */
            for (int i = n + 1; i > 0; --i)
                digits[i] = digits[i - 1];
            digits[0] = 1;
            ++n; /* number of digits increased by one */
        }
    }

    /* Convert the first n+1 digits to the return value */
    seq_t result = 0;
    for (int i = 0; i <= (int)n; ++i) {
        result = result * 10 + (seq_t)digits[i];
    }

    free(digits);
    return result;
}
