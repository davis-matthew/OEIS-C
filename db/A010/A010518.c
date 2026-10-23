/*
 * A010518  Decimal expansion of square root of 66.
 * https://oeis.org/A010518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 9971, u16 n <= 9971, u32 n <= 9971, u64 n <= 9971,
 *            u128 n <= 9971, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of sqrt(66), including integer part.
   The nth term a(n) is the nth digit of the decimal expansion of sqrt(66).
   Computation is performed by the standard digit‑by‑digit extraction
   algorithm.                                               */
#include <stdint.h>
#include <stddef.h>

/* ----- Big‑integer helper for the extraction routine ---------------- */

#define BIGINT_BASE 1000000000U          /* 1e9, fits in 32‑bit */
#define BIGINT_MAX_LIMBS 1200U           /* enough for ~10000 decimal digits */

typedef struct {
    size_t len;                         /* number of used limbs (0 means zero) */
    uint32_t d[BIGINT_MAX_LIMBS];
} BigInt;

static void bi_set_uint(BigInt *a, uint32_t v)
{
    if (v == 0) {
        a->len = 0;
        return;
    }
    a->len = 1;
    a->d[0] = v;
}

static void bi_copy(const BigInt *src, BigInt *dst)
{
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i)
        dst->d[i] = src->d[i];
}

static void bi_mul_small(BigInt *a, uint32_t m)
{
    if (m == 0 || a->len == 0) {
        a->len = 0;
        return;
    }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BIGINT_BASE);
        carry   = prod / BIGINT_BASE;
    }
    while (carry) {
        if (a->len >= BIGINT_MAX_LIMBS) return;  /* safety – should not happen */
        a->d[a->len++] = (uint32_t)(carry % BIGINT_BASE);
        carry /= BIGINT_BASE;
    }
}

static void bi_add_small(BigInt *a, uint32_t v)
{
    if (v == 0) return;
    uint64_t carry = v;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BIGINT_BASE);
        carry   = sum / BIGINT_BASE;
        ++i;
    }
    while (carry) {
        if (a->len >= BIGINT_MAX_LIMBS) return;  /* safety – should not happen */
        a->d[a->len++] = (uint32_t)carry;
        carry /= BIGINT_BASE;
    }
}

static int bi_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return a->len < b->len ? -1 : 1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i])
            return a->d[i] < b->d[i] ? -1 : 1;
    }
    return 0;
}

static void bi_sub(BigInt *a, const BigInt *b)   /* assumes a >= b */
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t temp = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - borrow;
        if (temp < 0) { temp += BIGINT_BASE; borrow = 1; }
        else          { borrow = 0; }
        a->d[i] = (uint32_t)temp;
    }
    while (a->len && a->d[a->len - 1] == 0) --a->len;
}

/* ---------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Integer part of sqrt(66) */
    const uint32_t INT_PART = 8U;          /* floor(sqrt(66))  */
    const uint32_t REM_INIT = 2U;          /* 66 - 8*8 = 2    */

    if (n == 1) return (seq_t)INT_PART;

    BigInt p, r;
    bi_set_uint(&p, INT_PART);   /* current root so far          */
    bi_set_uint(&r, REM_INIT);   /* current remainder            */

    int digit = 0;

    for (idx_t k = 2; k <= n; ++k) {
        bi_mul_small(&r, 100U);          /* bring down two zeros */

        int d;
        BigInt cand;
        for (d = 9; d >= 0; --d) {
            bi_copy(&p, &cand);
            bi_mul_small(&cand, 20U);
            bi_add_small(&cand, (uint32_t)d);
            bi_mul_small(&cand, (uint32_t)d);
            if (bi_cmp(&cand, &r) <= 0) break;
        }

        bi_sub(&r, &cand);               /* r = r - candidate      */
        bi_mul_small(&p, 10U);           /* p = p*10 + d           */
        bi_add_small(&p, (uint32_t)d);

        digit = d;                       /* store current digit    */
    }

    return (seq_t)digit;
}
