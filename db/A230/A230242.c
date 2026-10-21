/*
 * A230242  Decimal expansion of (25+3*sqrt(69))/2.
 * https://oeis.org/A230242
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit n of (25 + 3*sqrt(69))/2 (offset 2, so a(2)
 *            is the first digit 2 of the expansion)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define BASE 1000000000U          /* 1e9 */
#define MAX_LIMBS 2000            /* enough for >10000 decimal digits */

/* ----- small arbitrary‑precision integer ----- */
typedef struct {
    int len;                   /* number of used limbs, 0 means zero */
    uint32_t d[MAX_LIMBS];
} BigInt;

static void big_zero(BigInt *a) { a->len = 0; }

static void big_set_uint(BigInt *a, uint32_t v)
{
    if (v == 0) { a->len = 0; return; }
    a->len = 1;
    a->d[0] = v;
}

static void big_trim(BigInt *a)
{
    while (a->len > 0 && a->d[a->len - 1] == 0) a->len--;
}

static void big_copy(BigInt *dst, const BigInt *src)
{
    dst->len = src->len;
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

static int big_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (int i = a->len - 1; i >= 0; --i)
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    return 0;
}

static void big_add_small(BigInt *a, uint32_t add)
{
    uint64_t carry = add;
    int i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        ++i;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)carry;
        carry /= BASE;
    }
}

static void big_mul_small(BigInt *a, uint32_t mul)
{
    if (mul == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (int i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * mul + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    while (carry) {
        a->d[a->len++] = (uint32_t)carry;
        carry /= BASE;
    }
}

static void big_sub(BigInt *a, const BigInt *b)            /* a >= b */
{
    int64_t borrow = 0;
    for (int i = 0; i < a->len; ++i) {
        int64_t sub = (int64_t)a->d[i] - (i < (int)b->len ? b->d[i] : 0) - borrow;
        if (sub < 0) {
            sub += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)sub;
    }
    big_trim(a);
}

/* ----- sequence function ----- */
seq_t a(idx_t n)
{
    /* handle the integer part (digits 2 and 4) */
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* n >= 4: compute k = n-3 (the position in the fractional part) */
    idx_t k = n - 3;
    size_t groups = (size_t)(k + 2);          /* number of digit groups in the radicand */

    BigInt root, rem, c, t, prod;
    big_zero(&root);
    big_zero(&rem);

    int root_mod20 = 0;                       /* root modulo 20 as we build it */

    for (size_t gi = 0; gi < groups; ++gi) {
        uint32_t v;
        if (gi == 0)       v = 6;   /* first group is single digit '6' */
        else if (gi == 1)  v = 21;  /* second group is '21' */
        else              v = 0;    /* all following groups are '00' */

        /* remainder = remainder * 100 + v */
        big_mul_small(&rem, 100);
        if (v) big_add_small(&rem, v);

        /* c = root * 20 */
        big_copy(&c, &root);
        big_mul_small(&c, 20);

        /* find the largest digit d (0-9) such that (c + d)*d <= rem */
        int d;
        for (d = 9; d >= 0; --d) {
            big_copy(&t, &c);
            big_add_small(&t, (uint32_t)d); /* t = c + d */
            big_mul_small(&t, (uint32_t)d); /* prod = (c + d) * d */
            if (big_cmp(&t, &rem) <= 0) {
                prod = t;                 /* prod already computed */
                break;
            }
        }

        /* update remainder and root */
        big_sub(&rem, &prod);
        big_mul_small(&root, 10);
        big_add_small(&root, (uint32_t)d);

        /* keep root modulo 20 for the final digit */
        root_mod20 = (root_mod20 * 10 + d) % 20;
    }

    /* compute A mod 20; A = 25 * 10^k */
    uint32_t Amod20 = (k == 1) ? 10 : 0;  /* for k ≥ 2, 10^k ≡ 0 (mod 20) */
    uint32_t Xmod20 = (Amod20 + root_mod20) % 20;
    uint32_t digit = Xmod20 / 2;          /* floor((A+P)/2) mod 10 */

    return (seq_t)digit;
}
