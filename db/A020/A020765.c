/*
 * A020765  Decimal expansion of 1/sqrt(8).
 * https://oeis.org/A020765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = nth decimal digit of 1/√8 (decimal expansion of √2/4)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdint.h>   /* uint8_t, uint32_t, uint64_t */
#include <stdlib.h>   /* malloc, free */

/* ------------------- big‑integer in decimal digits ------------------- */
#define MAX_DEC 21000          /* enough for n up to ~10 000 */

typedef struct {
    uint8_t d[MAX_DEC];   /* little‑endian decimal digits */
    size_t len;           /* number of used digits (len>0) */
} DecNum;

/* initialise to zero */
static void dec_zero(DecNum *a)
{
    a->len = 0;
}

/* initialise from unsigned integer (≤ 2¹⁶⁴) */
static void dec_set_uint64(DecNum *a, uint64_t v)
{
    a->len = 0;
    if (v == 0) {
        a->d[0] = 0;
        a->len = 1;
        return;
    }
    while (v) {
        a->d[a->len++] = (uint8_t)(v % 10);
        v /= 10;
    }
}

/* compare: <0 if a<b, 0 if equal, >0 if a>b */
static int dec_cmp(const DecNum *a, const DecNum *b)
{
    if (a->len != b->len) return (a->len < b->len) ? -1 : 1;
    for (size_t i = a->len; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

/* multiply a by a small integer m (0 ≤ m ≤ 200) → res */
static void dec_mul_small(const DecNum *a, uint32_t m, DecNum *res)
{
    uint64_t carry = 0;
    res->len = a->len;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        res->d[i] = (uint8_t)(prod % 10);
        carry = prod / 10;
    }
    while (carry) {
        res->d[res->len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

/* multiply a in‑place by a small integer m */
static void dec_mul_small_assign(DecNum *a, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint8_t)(prod % 10);
        carry = prod / 10;
    }
    while (carry) {
        a->d[a->len++] = (uint8_t)(carry % 10);
        carry /= 10;
    }
}

/* add a small integer (≤ 200) to a in‑place */
static void dec_add_small_assign(DecNum *a, uint32_t add)
{
    uint64_t carry = add;
    size_t i = 0;
    while (carry) {
        if (i >= a->len) a->d[a->len++] = 0;
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint8_t)(sum % 10);
        carry = sum / 10;
        ++i;
    }
}

/* subtract b from a in‑place: a = a - b   (assume a >= b) */
static void dec_sub_inplace(DecNum *a, const DecNum *b)
{
    int borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int ai = a->d[i];
        int bi = (i < b->len) ? b->d[i] : 0;
        int diff = ai - bi - borrow;
        if (diff < 0) { diff += 10; borrow = 1; } else borrow = 0;
        a->d[i] = (uint8_t)diff;
    }
    while (a->len > 1 && a->d[a->len - 1] == 0) --a->len;
}

/* subtract a small integer from a in‑place */
static void dec_sub_small_assign(DecNum *a, uint32_t sub)
{
    DecNum tmp;
    dec_set_uint64(&tmp, sub);
    dec_sub_inplace(a, &tmp);
}

/* ------------------- compute digits of 1/√8 --------------------------- */
static void compute_digits(size_t size, seq_t *out_seq)
{
    size_t m = size;                 /* number of decimal digits required */
    static DecNum root, r, q, temp;
    dec_zero(&root);
    dec_zero(&r);
    dec_zero(&q);
    dec_zero(&temp);

    /* initialise r = 2, root = 1 (first integer digit of √2) */
    dec_set_uint64(&r, 2);
    dec_set_uint64(&root, 1);

    /* root*20 in q will be recomputed each step, so no need to init q */

    /* array holding the digits of X = floor(√2·10ᵐ)  (most‑significant first) */
    uint8_t *Xdigits = (uint8_t *)malloc(m + 1);
    Xdigits[0] = 1;                    /* first integer digit */

    /* subtract 1² from r: r = r - 1 */
    dec_sub_small_assign(&r, 1);

    /* Generate the remaining m digits of √2 */
    for (size_t step = 1; step <= m; ++step) {
        /* r = r * 100 */
        dec_mul_small_assign(&r, 100);

        /* q = root * 20 */
        dec_mul_small(&root, 20, &q);

        /* find largest d (0–9) such that (q + d) * d ≤ r */
        int best_d = 0;
        for (int d = 9; d >= 0; --d) {
            /* temp = q * d */
            dec_mul_small(&q, d, &temp);
            /* temp += d*d */
            dec_add_small_assign(&temp, (uint32_t)(d * d));
            if (dec_cmp(&temp, &r) <= 0) {
                best_d = d;
                break;
            }
        }

        /* Now temp = q * best_d + best_d² */
        dec_mul_small(&q, best_d, &temp);
        dec_add_small_assign(&temp, (uint32_t)(best_d * best_d));

        /* r = r - temp */
        dec_sub_inplace(&r, &temp);

        /* root = root * 10 + best_d */
        dec_mul_small_assign(&root, 10);
        dec_add_small_assign(&root, (uint32_t)best_d);

        /* store the digit */
        Xdigits[step] = (uint8_t)best_d;
    }

    /* Divide X by 4 to obtain Y = floor(√2·10ᵐ / 4)  (m digits) */
    uint8_t *Ydigits = (uint8_t *)malloc(m);
    int carry = 0;
    size_t ypos = 0;
    for (size_t i = 0; i <= m; ++i) {
        int cur = carry * 10 + Xdigits[i];
        int qd = cur / 4;
        carry = cur % 4;
        if (ypos == 0 && qd == 0) {
            /* skip leading zero */
            continue;
        }
        Ydigits[ypos++] = (uint8_t)qd;
    }
    /* ypos should equal m */

    /* Copy digits to output sequence array */
    for (size_t i = 0; i < size; ++i) {
        out_seq[i] = (seq_t)Ydigits[i];
    }

    free(Xdigits);
    free(Ydigits);
}

/* ------------------- public sequence function ------------------------ */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    size_t needed = (size_t)(n + 1);

    if (needed > cache_size) {
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        compute_digits(needed, new_cache);
        if (cache) free(cache);
        cache = new_cache;
        cache_size = needed;
    }
    return cache[(size_t)n];
}
