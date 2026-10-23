/*
 * A140249  Decimal expansion of 3*sqrt(15)/2.
 * https://oeis.org/A140249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = floor( (3*sqrt(15)/2) * 10^(n-1) ) mod 10 (decimal digits
 *            of 3*sqrt(15)/2, offset 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* ---------- Big integer type (base 1e9) ---------- */
#define BI_BASE 1000000000U
#define BI_MAX_LIMBS 2000   /* enough for ≈ 18000 decimal digits */

typedef struct {
    uint32_t d[BI_MAX_LIMBS];
    size_t   len;   /* number of used limbs */
} BigInt;

/* basic operations ------------------------------------------------------- */
static void bi_zero   (BigInt *a) { a->len = 0; }

static void bi_set_small(BigInt *a, uint32_t v)
{
    if (v == 0) { a->len = 0; }
    else { a->len = 1; a->d[0] = v; }
}

static void bi_copy   (BigInt *dst, const BigInt *src)
{
    dst->len = src->len;
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
}

/* comparison: <0 if a<b, 0 if equal, >0 if a>b */
static int bi_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* multiply by small integer (0 <= m < BI_BASE) */
static void bi_mul_small(BigInt *a, uint32_t m)
{
    if (m == 0 || a->len == 0) { a->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BI_BASE);
        carry   = prod / BI_BASE;
    }
    while (carry) {
        if (a->len >= BI_MAX_LIMBS) break;   /* safety */
        a->d[a->len++] = (uint32_t)(carry % BI_BASE);
        carry /= BI_BASE;
    }
}

/* add small integer (0 <= add < BI_BASE) */
static void bi_add_small(BigInt *a, uint32_t add)
{
    if (add == 0) return;
    uint64_t carry = add;
    size_t i = 0;
    while (carry && i < a->len) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BI_BASE);
        carry   = sum / BI_BASE;
        ++i;
    }
    if (carry) {
        if (a->len < BI_MAX_LIMBS) {
            a->d[a->len++] = (uint32_t)carry;
        }
    }
}

/* a = a - b   (assumes a >= b) */
static void bi_sub(BigInt *a, const BigInt *b)
{
    uint64_t borrow = 0;
    size_t i = 0;
    for (; i < a->len; ++i) {
        uint64_t ai = a->d[i];
        uint64_t bi = (i < b->len) ? b->d[i] : 0;
        uint64_t diff = ai - bi - borrow;
        if (ai < bi + borrow) { borrow = 1; diff += BI_BASE; }
        else borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (borrow && a->len > 0) {
        --a->len;
        if (a->len && a->d[a->len] < borrow) {
            a->d[a->len] = (uint32_t)(BI_BASE - borrow);
            borrow = 1;
        } else {
            a->d[a->len] -= (uint32_t)borrow;
            borrow = 0;
        }
    }
    while (a->len > 0 && a->d[a->len-1] == 0) --a->len;
}

/* ---------- sequence implementation ---------- */
#define MAX_N 10001   /* maximum n that will be queried */

static int32_t cache[MAX_N];   /* cache of computed a(n) */
static size_t  cache_len = 0;  /* maximum n cached (1‑based) */

static BigInt root, remainder, temp;
static int root_mod20 = 0;     /* root modulo 20 */
static size_t processed_groups = 0; /* number of groups processed, equals cache_len+1 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1 */

    /* extend cache if necessary */
    while (processed_groups <= n) {
        /* determine current group value */
        uint32_t group;
        if (processed_groups == 0) group = 1;
        else if (processed_groups == 1) group = 35;
        else group = 0;   /* subsequent groups are 00 */

        /* remainder = remainder*100 + group */
        bi_mul_small(&remainder, 100);
        bi_add_small(&remainder, group);

        /* find digit d (0..9) */
        int d;
        for (d = 9; d >= 0; --d) {
            /* temp = (root*20 + d) * d */
            bi_copy(&temp, &root);
            bi_mul_small(&temp, 20);
            bi_add_small(&temp, (uint32_t)d);
            bi_mul_small(&temp, (uint32_t)d);
            if (bi_cmp(&temp, &remainder) <= 0) break;
        }

        /* remainder -= temp */
        bi_sub(&remainder, &temp);

        /* root = root*10 + d */
        bi_mul_small(&root, 10);
        bi_add_small(&root, (uint32_t)d);

        /* update root modulo 20 */
        root_mod20 = (root_mod20 * 10 + d) % 20;

        /* compute a for current n = processed_groups */
        int parity = root_mod20 & 1;
        int q = (root_mod20 - parity) / 2;   /* floor(root/2) modulo 20 */
        int digit = q % 10;
        cache[processed_groups] = digit;

        processed_groups++;
    }

    return (seq_t)cache[(size_t)n];
}
