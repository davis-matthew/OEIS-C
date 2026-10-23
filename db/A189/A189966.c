/*
 * A189966  Decimal expansion of (3+sqrt(33))/4, which has periodic continued
 *          fractions [2,5,2,1,2,5,2,1,...] and [3/2, 1, 3/2, 1, ...].
 * https://oeis.org/A189966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth decimal digit of (3+√33)/4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by integer digit‑by‑digit square‑root algorithm and modulo arithmetic */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Base for big‑integer limbs (fits in 32 bits) */
#define BASE 1000000000u

/* Simple non‑negative big integer (little‑endian limbs) */
typedef struct {
    uint32_t *d;   /* limbs */
    size_t   len;  /* number of limbs actually used */
    size_t   cap;  /* allocated capacity */
} BigInt;

/* Ensure that capacity is at least cap */
static void BigInt_ensure_cap(BigInt *bi, size_t cap)
{
    if (bi->cap < cap) {
        size_t new_cap = bi->cap * 2;
        if (new_cap < cap) new_cap = cap;
        uint32_t *newd = (uint32_t *)realloc(bi->d, new_cap * sizeof(uint32_t));
        if (!newd) exit(1);
        bi->d = newd;
        bi->cap = new_cap;
    }
}

/* Initialise from a 32‑bit value */
static void BigInt_init(BigInt *bi, uint32_t val)
{
    bi->len = (val == 0 ? 0 : 1);
    bi->cap = 1;
    bi->d = (uint32_t *)malloc(bi->cap * sizeof(uint32_t));
    if (!bi->d) exit(1);
    bi->d[0] = val;
}

/* Free the storage */
static void BigInt_free(BigInt *bi)
{
    free(bi->d);
    bi->d   = NULL;
    bi->len = bi->cap = 0;
}

/* Remove trailing zero limbs */
static void BigInt_normalize(BigInt *bi)
{
    while (bi->len > 0 && bi->d[bi->len - 1] == 0) bi->len--;
}

/* Multiply by a small integer (≤ 2²⁶‑1) */
static void BigInt_mul_small(BigInt *bi, uint32_t m)
{
    if (m == 0) {
        bi->len = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < bi->len; ++i) {
        uint64_t prod = (uint64_t)bi->d[i] * m + carry;
        bi->d[i]     = (uint32_t)(prod % BASE);
        carry        = prod / BASE;
    }
    while (carry) {
        BigInt_ensure_cap(bi, bi->len + 1);
        bi->d[bi->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
    BigInt_normalize(bi);
}

/* Add a small integer (≤ BASE‑1) */
static void BigInt_add_small(BigInt *bi, uint32_t a)
{
    uint64_t carry = a;
    size_t i = 0;
    while (carry) {
        if (i == bi->len) {
            BigInt_ensure_cap(bi, bi->len + 1);
            bi->d[bi->len++] = 0;
        }
        uint64_t sum = (uint64_t)bi->d[i] + carry;
        bi->d[i]     = (uint32_t)(sum % BASE);
        carry        = sum / BASE;
        ++i;
    }
    BigInt_normalize(bi);
}

/* Copy one big integer into another */
static void BigInt_copy(BigInt *dest, const BigInt *src)
{
    BigInt_ensure_cap(dest, src->len);
    memcpy(dest->d, src->d, src->len * sizeof(uint32_t));
    dest->len = src->len;
    BigInt_normalize(dest);
}

/* Compare two big integers: return -1,0,1 */
static int BigInt_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (size_t i = a->len; i > 0; --i) {
        if (a->d[i - 1] != b->d[i - 1])
            return a->d[i - 1] < b->d[i - 1] ? -1 : 1;
    }
    return 0;
}

/* Subtract b from a (assumes a ≥ b) */
static void BigInt_sub(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    size_t i = 0;
    for (; i < b->len || borrow; ++i) {
        int64_t sub = (int64_t)(i < a->len ? a->d[i] : 0) -
                      (int64_t)(i < b->len ? b->d[i] : 0) -
                      borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else
            borrow = 0;
        if (i < a->len)
            a->d[i] = (uint32_t)sub;
        else {
            BigInt_ensure_cap(a, i + 1);
            a->d[i] = (uint32_t)sub;
            a->len = i + 1;
        }
    }
    BigInt_normalize(a);
}

/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n is 1‑based index of the decimal expansion */
    idx_t m = n - 1;           /* number of digits after the point needed */

    /* Big integers: P – current square‑root prefix,
       R – current remainder,
       temp – auxiliary workspace,
       prod – product (20*P + d)*d. */
    BigInt P, R, temp, prod;
    BigInt_init(&P, 5);                     /* floor(√33) = 5 */
    BigInt_init(&R, 8);                     /* 33 – 5² = 8 */
    BigInt_init(&temp, 0);
    BigInt_init(&prod, 0);

    /* Keep track of P modulo 4 and modulo 40 (needed only for the final digit) */
    uint32_t p_mod4   = 5 % 4;
    uint32_t p_mod40  = 5 % 40;

    for (idx_t step = 0; step < m; ++step) {
        /* R ← 100·R */
        BigInt_mul_small(&R, 100);

        /* temp ← 20·P */
        BigInt_copy(&temp, &P);
        BigInt_mul_small(&temp, 20);

        int dfound = 0;
        /* Test digits 9 … 0 to find the largest that fits */
        for (int d = 9; d >= 0; --d) {
            /* prod ← (temp + d)·d = temp*d + d² */
            BigInt_copy(&prod, &temp);
            BigInt_mul_small(&prod, d);                 /* temp*d */
            BigInt_add_small(&prod, (uint32_t)(d * d));  /* + d*d */

            if (BigInt_cmp(&prod, &R) <= 0) {
                dfound = d;
                break;
            }
        }

        /* R ← R – prod */
        BigInt_sub(&R, &prod);

        /* P ← 10·P + dfound */
        BigInt_mul_small(&P, 10);
        BigInt_add_small(&P, dfound);

        /* Update the modular values of P */
        p_mod4  = (p_mod4  * 10 + dfound) % 4;
        p_mod40 = (p_mod40 * 10 + dfound) % 40;
    }

    /* 3·10^m modulo 4 and modulo 40 */
    uint32_t add_mod4;
    uint32_t add_mod40;
    if (m == 0) {
        add_mod4  = 3;
        add_mod40 = 3;
    } else if (m == 1) {
        add_mod4  = 0;
        add_mod40 = 30;
    } else if (m == 2) {
        add_mod4  = 0;
        add_mod40 = 20;
    } else {
        add_mod4  = 0;
        add_mod40 = 0;
    }

    uint32_t N_mod4   = (p_mod4 + add_mod4) % 4;
    uint32_t N_mod40  = (p_mod40 + add_mod40) % 40;

    /* (N mod 40 – N mod 4) is a multiple of 4; divide by 4 and take last decimal digit */
    uint32_t diff  = N_mod40 - N_mod4;   /* 0 … 36 */
    uint32_t digit = (diff / 4) % 10;

    /* Clean up */
    BigInt_free(&P);
    BigInt_free(&R);
    BigInt_free(&temp);
    BigInt_free(&prod);

    return (seq_t)digit;
}
