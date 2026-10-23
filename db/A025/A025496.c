/*
 * A025496  Number of terms in Zeckendorf representation of 4^n.
 * https://oeis.org/A025496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of terms in the Zeckendorf representation of 4^n
 *            (gotten via a greedy decomposition of 4^n expressed in Fibonacci
 *            numbers F1 = 1, F2 = 2, F_k = F_{k-1} + F_{k-2})
 * Range:     u8 n <= 306, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* ---------- Tiny big‑integer library ---------- */
typedef struct {
    size_t sz;          /* number of 32‑bit limbs (little endian) */
    uint32_t *limb;     /* pointer to limb array */
} bigint;

/* create a bigint value from an unsigned 32‑bit integer */
static void bi_from_uint32(uint32_t v, bigint *b)
{
    b->sz = 1;
    b->limb = (uint32_t *)malloc(sizeof(uint32_t));
    b->limb[0] = v;
}

/* copy a bigint */
static void bi_assign(bigint *dest, const bigint *src)
{
    dest->sz = src->sz;
    dest->limb = (uint32_t *)malloc(dest->sz * sizeof(uint32_t));
    memcpy(dest->limb, src->limb, dest->sz * sizeof(uint32_t));
}

/* free a bigint */
static void bi_free(bigint *b)
{
    if (b->limb) {
        free(b->limb);
        b->limb = NULL;
    }
    b->sz = 0;
}

/* compare two bigints: return -1 if a<b, 0 if a==b, +1 if a>b */
static int bi_cmp(const bigint *a, const bigint *b)
{
    if (a->sz > b->sz) return 1;
    if (a->sz < b->sz) return -1;
    for (size_t i = a->sz; i-- > 0; ) {
        if (a->limb[i] > b->limb[i]) return 1;
        if (a->limb[i] < b->limb[i]) return -1;
    }
    return 0;
}

/* big integer addition: c = a + b (c must be freshly allocated) */
static void bi_add(const bigint *a, const bigint *b, bigint *c)
{
    size_t maxsz = a->sz > b->sz ? a->sz : b->sz;
    c->sz = maxsz + 1;
    c->limb = (uint32_t *)malloc(c->sz * sizeof(uint32_t));
    uint64_t carry = 0;
    for (size_t i = 0; i < maxsz; ++i) {
        uint64_t av = (i < a->sz) ? a->limb[i] : 0;
        uint64_t bv = (i < b->sz) ? b->limb[i] : 0;
        uint64_t sum = av + bv + carry;
        c->limb[i] = (uint32_t)sum;
        carry = sum >> 32;
    }
    if (carry) {
        c->limb[maxsz] = (uint32_t)carry;
    } else {
        c->sz--;                     /* trim top zero limb */
    }
}

/* big integer subtraction in place: a -= b   (assumes a >= b) */
static void bi_sub_in_place(bigint *a, const bigint *b)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->sz; ++i) {
        int64_t av = a->limb[i];
        int64_t bv = (i < b->sz) ? b->limb[i] : 0;
        int64_t diff = av - bv - borrow;
        if (diff < 0) {
            diff += ((int64_t)1 << 32);
            borrow = 1;
        } else {
            borrow = 0;
        }
        a->limb[i] = (uint32_t)diff;
    }
    /* trim leading zero limbs */
    while (a->sz > 1 && a->limb[a->sz - 1] == 0)
        a->sz--;
}

/* create a big integer equal to 1 << shift (shift in bits) */
static void bi_bitshift_left(uint64_t shift, bigint *b)
{
    size_t idx = shift / 32;
    uint32_t bits = (uint32_t)(shift % 32);
    b->sz = idx + 1;
    b->limb = (uint32_t *)calloc(b->sz, sizeof(uint32_t));
    b->limb[idx] = 1U << bits;
}

/* ---------- Fibonacci cache ---------- */
static bigint *fibs = NULL;
static size_t fibs_len = 0;
static size_t fibs_cap = 0;

/* ensure that fibs contains all Fibonacci numbers <= target */
static void ensure_fibs_up_to(const bigint *target)
{
    /* initialize if first call */
    if (fibs == NULL) {
        fibs_cap = 8;
        fibs = (bigint *)malloc(fibs_cap * sizeof(bigint));
        /* F1 = 1, F2 = 2 */
        bi_from_uint32(1, &fibs[0]);
        bi_from_uint32(2, &fibs[1]);
        fibs_len = 2;
    }
    /* generate until last Fibonacci > target */
    while (bi_cmp(&fibs[fibs_len - 1], target) <= 0) {
        if (fibs_len == fibs_cap) {
            fibs_cap *= 2;
            fibs = (bigint *)realloc(fibs, fibs_cap * sizeof(bigint));
        }
        bi_add(&fibs[fibs_len - 1], &fibs[fibs_len - 2], &fibs[fibs_len]);
        fibs_len++;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* 4^n = 2^(2n) */
    uint64_t shift = n << 1;      /* shift = 2*n (fits in 64 bits for the tested range) */
    bigint target;
    bi_bitshift_left(shift, &target);

    /* generate Fibonacci numbers up to 4^n */
    ensure_fibs_up_to(&target);

    /* greedy Zeckendorf decomposition */
    bigint rem;
    bi_assign(&rem, &target);

    size_t idx = fibs_len - 1;
    size_t count = 0;

    while (!(rem.sz == 1 && rem.limb[0] == 0)) {
        while (bi_cmp(&fibs[idx], &rem) > 0)
            idx--;
        bi_sub_in_place(&rem, &fibs[idx]);
        count++;
    }

    /* clean up temporary bigints */
    bi_free(&rem);
    bi_free(&target);

    return (seq_t)count;
}
