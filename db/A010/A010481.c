/*
 * A010481  Decimal expansion of square root of 26.
 * https://oeis.org/A010481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9916, u16 n <= 9916, u32 n <= 9916, u64 n <= 9916,
 *            u128 n <= 9916, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: digits of sqrt(26) (decimal expansion) */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Big integer in base BASE, little‑endian */
#define MAX_DIGITS 1200          /* enough for 10000 decimal digits in base 1e9 */
#define BASE 1000000000U         /* 1e9 */

typedef struct {
    uint32_t d[MAX_DIGITS];
    size_t len;
} BigInt;

/* Remove leading zero groups */
static void bi_norm(BigInt *x) {
    while (x->len > 0 && x->d[x->len-1] == 0)
        x->len--;
}

/* Copy big integer */
static void bi_copy(BigInt *dest, const BigInt *src) {
    dest->len = src->len;
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
}

/* Multiply by small integer (m <= 100) */
static void bi_mul_small(BigInt *x, unsigned int m) {
    if (m == 0) { x->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Add small integer (a <= 100) */
static void bi_add_small(BigInt *x, unsigned int a) {
    if (a == 0) return;
    uint64_t carry = a;
    for (size_t i = 0; i < x->len && carry; ++i) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
    }
    if (carry) {
        x->d[x->len++] = (uint32_t)carry;
    }
}

/* Subtract b from a (a >= b) */
static void bi_sub(BigInt *a, const BigInt *b) {
    int64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) + carry;
        if (sub < 0) {
            sub += BASE;
            carry = -1;
        } else {
            carry = 0;
        }
        a->d[i] = (uint32_t)sub;
    }
    bi_norm(a);
}

/* Compare a and b: return -1 if a<b, 0 if equal, 1 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b) {
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (int i = (int)a->len-1; i >= 0; --i) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* Return the n‑th decimal digit of sqrt(26) (n ≥ 1) */
seq_t a(idx_t n) {
    /* Memoised state: digits already computed */
    static seq_t *digits = NULL;      /* array of digits, 0‑indexed */
    static size_t digits_cap = 0;     /* allocated capacity */
    static idx_t digits_calc = 0;     /* number of digits stored */

    /* Big integer state for the algorithm */
    static BigInt S, R;               /* current partial root and remainder */
    static int initialized = 0;       /* 0 → not yet set up */

    /* Allocate/resize memoisation array if necessary */
    if (n > digits_cap) {
        size_t new_cap = digits_cap ? digits_cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure → return 0 */
        digits = tmp;
        digits_cap = new_cap;
    }

    /* If requested digit already known, return it */
    if (n <= digits_calc)
        return digits[n-1];

    /* Initialise state on first call */
    if (!initialized) {
        /* Integer part of sqrt(26) */
        S.len = 1;
        S.d[0] = 5;
        R.len = 1;
        R.d[0] = 26 - 5 * 5;          /* 1 */
        digits[0] = 5;
        digits_calc = 1;
        initialized = 1;
    }

    /* Compute further digits up to n */
    for (idx_t cur = digits_calc + 1; cur <= n; ++cur) {
        /* Bring down next decimal group '00' */
        bi_mul_small(&R, 100);

        /* divisor = S * 20 */
        BigInt divisor;
        bi_copy(&divisor, &S);
        bi_mul_small(&divisor, 20);

        /* Find largest x in 0..9 with (divisor + x) * x <= R */
        int x = 0;
        for (int cand = 9; cand >= 0; --cand) {
            BigInt tmp;
            bi_copy(&tmp, &divisor);
            bi_add_small(&tmp, cand);
            bi_mul_small(&tmp, cand);
            if (bi_cmp(&tmp, &R) <= 0) {
                x = cand;
                break;
            }
        }

        /* Append x to the root: S = S * 10 + x */
        bi_mul_small(&S, 10);
        bi_add_small(&S, (unsigned int)x);

        /* Subtract the value found from the remainder */
        BigInt rem_sub;
        bi_copy(&rem_sub, &divisor);
        bi_add_small(&rem_sub, (unsigned int)x);
        bi_mul_small(&rem_sub, (unsigned int)x);
        bi_sub(&R, &rem_sub);

        /* Store the digit */
        digits[digits_calc++] = (seq_t)x;
    }

    return digits[n-1];
}
