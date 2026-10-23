/*
 * A010486  Decimal expansion of square root of 31.
 * https://oeis.org/A010486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(sqrt(31) * 10^(n-1)) mod 10 (digits of the decimal
 *            expansion of √31, offset 1)
 * Range:     u8 n <= 9843, u16 n <= 9843, u32 n <= 9843, u64 n <= 9843,
 *            u128 n <= 9843, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Map for a big‑integer in base 1 000 000 000 */
#define BASE 1000000000U
#define MAXLIMBS 2000   /* sufficient for n up to ~18000 digits */

/* Simple big‑integer structure (little‑endian limbs) */
typedef struct {
    uint32_t *limb;
    size_t len;
} BigInt;

/* Multiply a by a small unsigned integer m (m ≤ 200). */
static void bi_mul_small(BigInt *a, uint32_t m)
{
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        a->limb[a->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Add a small unsigned integer v (v ≤ 9). */
static void bi_add_small(BigInt *a, uint32_t v)
{
    if (v == 0) return;
    size_t i = 0;
    uint64_t carry = v;
    while (carry) {
        if (i >= a->len) a->limb[a->len++] = 0;
        uint64_t sum = (uint64_t)a->limb[i] + carry;
        a->limb[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        ++i;
    }
}

/* Subtract b from a (assume a ≥ b). */
static void bi_sub_assign(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    size_t i = 0;
    for (; i < b->len || borrow; ++i) {
        int64_t diff = (int64_t)a->limb[i]
                     - (i < b->len ? b->limb[i] : 0)
                     - borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->limb[i] = (uint32_t)diff;
    }
    /* Remove leading zero limbs */
    while (a->len > 0 && a->limb[a->len - 1] == 0) --a->len;
}

/* Compare two big integers: return -1 if a<b, 0 if a==b, +1 if a>b. */
static int bi_compare(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->limb[i] != b->limb[i]) return (a->limb[i] < b->limb[i]) ? -1 : 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    static uint32_t memR[MAXLIMBS];
    static uint32_t memRes[MAXLIMBS];
    static uint32_t memProd[MAXLIMBS];
    BigInt R, Res, Prod;

    /* Initialise R = 31 */
    R.limb = memR;  R.len = 0;
    uint64_t tmp = 31;
    while (tmp) { R.limb[R.len++] = (uint32_t)(tmp % BASE); tmp /= BASE; }

    /* Initialise Res = 0 */
    Res.limb = memRes;  Res.len = 0;

    for (idx_t step = 1; step <= n; ++step) {
        /* Find the largest digit x (0..9) such that (20*Res + x)*x <= R */
        int x;
        for (x = 9; x >= 0; --x) {
            /* Prod = Res*20 + x */
            Prod.limb = memProd;  Prod.len = Res.len;
            memcpy(Prod.limb, Res.limb, Res.len * sizeof(uint32_t));
            bi_mul_small(&Prod, 20);      /* Prod = Res*20 */
            bi_add_small(&Prod, (uint32_t)x); /* Prod += x */
            bi_mul_small(&Prod, (uint32_t)x); /* Prod *= x */
            if (bi_compare(&Prod, &R) <= 0) break;
        }

        /* Update Res: Res = Res*10 + x */
        bi_mul_small(&Res, 10);
        bi_add_small(&Res, (uint32_t)x);

        /* R = R - Prod */
        bi_sub_assign(&R, &Prod);

        /* Bring down next pair of zeros (unless last step) */
        if (step < n) bi_mul_small(&R, 100);
    }

    /* The requested digit is the last decimal digit of Res */
    uint32_t digit = Res.limb[0] % 10;
    return (seq_t)digit;
}
