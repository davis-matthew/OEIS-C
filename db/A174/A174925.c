/*
 * A174925  Decimal expansion of (2+sqrt(6))/4.
 * https://oeis.org/A174925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = floor((2+sqrt(6))/4 * 10^(n-1)) mod 10
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses a digit‑by‑digit algorithm for sqrt(6) together with
 * a small modulus trick to get the decimal digit without large
 * intermediate integers. The only big‑integer arithmetic is used
 * to compute sqrt(6) * 10^(n-1) modulo 40, which is sufficient
 * for the final digit. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

#define BASE 1000000000U   /* 1e9 */
typedef struct {
    uint32_t *d;          /* little‑endian digits, d[0] least significant */
    size_t len;           /* number of used limbs */
} BigInt;

/* initialise a BigInt from a small integer */
static void bi_init(BigInt *x, uint32_t v)
{
    x->len = 0;
    if (v == 0) return;
    if (v >= BASE) {
        x->d[x->len++] = v % BASE;
        v /= BASE;
        if (v != 0) x->d[x->len++] = v;
    } else {
        x->d[x->len++] = v;
    }
}

/* trim unused high limbs */
static void bi_trim(BigInt *x)
{
    while (x->len > 0 && x->d[x->len - 1] == 0) x->len--;
}

/* compare two BigInts: return -1 if a<b, 0 if equal, 1 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* multiply BigInt by small integer m (m < BASE) */
static void bi_mul_small(BigInt *x, uint32_t m)
{
    if (m == 0 || x->len == 0) { x->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    if (carry) {
        x->d[x->len++] = (uint32_t)carry;
    }
}

/* add small integer m to BigInt */
static void bi_add_small(BigInt *x, uint32_t m)
{
    uint64_t carry = m;
    for (size_t i = 0; i < x->len && carry; ++i) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BASE);
        carry   = sum / BASE;
    }
    if (carry) {
        x->d[x->len++] = (uint32_t)carry;
    }
}

/* subtract BigInt b from a; assumes a >= b */
static void bi_sub(const BigInt *a, const BigInt *b, BigInt *res)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t sub = (uint64_t)a->d[i] - borrow
                       - (i < b->len ? b->d[i] : 0);
        if (sub >> 63) {                 /* if the high bit set, we subtracted too much */
            sub += BASE;
            borrow = 1;
        } else borrow = 0;
        res->d[i] = (uint32_t)sub;
    }
    res->len = a->len;
    bi_trim(res);
}

/* copy BigInt src into dst; dst must have capacity >= src->len */
static void bi_copy(const BigInt *src, BigInt *dst)
{
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

/* the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* dummy for safety */

    /* if n == 1 we only need the integer part */
    if (n == 1) return 1;

    idx_t m = n - 1;                      /* number of decimal places needed */

    /* maximum number of limbs needed for sqrt(6) * 10^m:
       each limb holds 9 decimal digits -> ceil((m+1)/9) + a few safety */
    size_t max_limbs = (m + 10) / 9 + 5;
    uint32_t *root_buf  = (uint32_t *)calloc(max_limbs, sizeof(uint32_t));
    uint32_t *rem_buf   = (uint32_t *)calloc(max_limbs, sizeof(uint32_t));
    uint32_t *temp_buf  = (uint32_t *)calloc(max_limbs, sizeof(uint32_t));
    uint32_t *twenty_buf= (uint32_t *)calloc(max_limbs, sizeof(uint32_t));
    uint32_t *cand_buf  = (uint32_t *)calloc(max_limbs, sizeof(uint32_t));

    BigInt root  = { root_buf, 0 };
    BigInt rem   = { rem_buf, 0 };
    BigInt temp  = { temp_buf, 0 };
    BigInt twenty_root = { twenty_buf, 0 };
    BigInt candidate   = { cand_buf, 0 };

    /* initialise root=2, remainder = 6 - 2^2 = 2 */
    bi_init(&root, 2);
    bi_init(&rem, 2);
    uint32_t root_mod40 = 2;             /* keep modulo 40 separately */

    for (idx_t i = 0; i < m; ++i) {
        /* temp = rem * 100 */
        bi_copy(&rem, &temp);
        bi_mul_small(&temp, 100);

        /* twenty_root = root * 20 */
        bi_copy(&root, &twenty_root);
        bi_mul_small(&twenty_root, 20);

        /* find largest d such that (twenty_root + d)*d <= temp */
        int d;
        for (d = 9; d >= 0; --d) {
            /* candidate = (twenty_root + d) * d */
            bi_copy(&twenty_root, &candidate);
            bi_add_small(&candidate, (uint32_t)d);
            bi_mul_small(&candidate, (uint32_t)d);

            if (bi_cmp(&candidate, &temp) <= 0) break;
        }

        /* rem = temp - candidate */
        bi_sub(&temp, &candidate, &rem);

        /* root = root * 10 + d */
        bi_mul_small(&root, 10);
        bi_add_small(&root, (uint32_t)d);

        /* update root_mod40 */
        root_mod40 = (root_mod40 * 10 + (uint32_t)d) % 40;
    }

    /* compute 2*10^(m) mod 40 */
    uint32_t pow10_mod40;
    if (m == 0)         pow10_mod40 = 1 % 40;
    else if (m == 1)    pow10_mod40 = 10 % 40;
    else if (m == 2)    pow10_mod40 = 20 % 40;
    else                pow10_mod40 = 0;   /* for m >= 3, 10^m ≡ 0 (mod 40) */

    uint32_t add_mod = (2u * pow10_mod40) % 40;
    uint32_t num_mod = (root_mod40 + add_mod) % 40;
    uint32_t digit = num_mod / 4;            /* because floor(n/4) mod 10 is (n mod 40)/4 */

    free(root_buf);
    free(rem_buf);
    free(temp_buf);
    free(twenty_buf);
    free(cand_buf);

    return (seq_t)digit;
}
