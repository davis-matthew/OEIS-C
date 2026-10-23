/*
 * A024505  a(n) = [ C(2n,n)/2^(n+2) ].
 * https://oeis.org/A024505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( binomial(2n,n) / 2^(n+2) )
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef unsigned long long uint64_ex;

/*====================================================================
 *  Small big‑integer library (base 2^32) – enough for n ≤ 10000
 *====================================================================*/
typedef struct {
    uint32_t *limb;   /* little endian limbs (little–significant first) */
    size_t   len;     /* number of used limbs (len==0 ⇒ value 0) */
} BigInt;

static void bigint_init(BigInt *a) { a->len = 0; a->limb = NULL; }

static void bigint_free(BigInt *a)
{
    free(a->limb);
    a->len = 0;
    a->limb = NULL;
}

/* set to unsigned integer v (0 ≤ v ≤ 2^64–1) */
static void bigint_set_uint(BigInt *a, uint64_t v)
{
    if (v == 0) { bigint_free(a); return; }
    a->len  = 0;
    a->limb = NULL;
    while (v) {
        if (a->len == 0) {
            a->limb = (uint32_t *)malloc(sizeof(uint32_t));
        } else {
            a->limb = (uint32_t *)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
        }
        a->limb[ a->len++ ] = (uint32_t)(v & 0xffffffffu);
        v >>= 32;
    }
}

/* multiply by small integer m (m < 2^32) */
static void bigint_mul_uint(BigInt *a, uint32_t m)
{
    if (m == 0 || a->len == 0) { bigint_free(a); return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->limb[i] * m + carry;
        a->limb[i]    = (uint32_t)prod;
        carry         = prod >> 32;
    }
    if (carry) {
        a->limb = (uint32_t *)realloc(a->limb, (a->len + 1) * sizeof(uint32_t));
        a->limb[ a->len++ ] = (uint32_t)carry;
    }
}

/* divide by small integer d (d < 2^32), return remainder */
static uint32_t bigint_div_uint(BigInt *a, uint32_t d)
{
    uint64_t rem = 0;
    for (size_t i = a->len; i > 0; --i) {
        size_t idx = i - 1;
        uint64_t cur = (rem << 32) | a->limb[idx];
        uint64_t q   = cur / d;
        rem          = cur % d;
        a->limb[idx] = (uint32_t)q;
    }
    while (a->len > 0 && a->limb[a->len-1] == 0) {
        --a->len;
    }
    if (a->len == 0) {
        free(a->limb);
        a->limb = NULL;
    }
    return (uint32_t)rem;
}

/* convert BigInt to seq_t – works for all seq_t sizes */
static seq_t bigint_to_seq_t(const BigInt *a)
{
    /* If seq_t fits in 64 bits, we can simply take the low 64 bits. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 val = 0;
        for (int i = (int)a->len - 1; i >= 0; --i) {
            val = (val << 32) | a->limb[i];
        }
        return (seq_t)(uint64_t)val;
    }

    /* For larger seq_t (arbitrary precision) use repeated *base + limb. */
    seq_t result = 0;
    /* base ≈ 2^BASE_BITS, chosen so that base fits into seq_t. */
    seq_t base;
    if (sizeof(seq_t) <= 4) {
        /* seq_t is 32 bit or less – shift by 16 bits twice iteration. */
        base = (seq_t)65536U;               /* 2^16 */
    } else {
        base = (seq_t)((uint64_t)1 << 32);  /* 2^32 fits into 64 bit */
    }

    for (size_t i = 0; i < a->len; ++i) {
        result = result * base + a->limb[i];
    }
    return result;
}

/*====================================================================
 *  Sequence function
 *====================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    BigInt binom;
    bigint_init(&binom);
    bigint_set_uint(&binom, 1);

    /* compute C(2n,n) multiplicatively: binom = Π (n+k)/k for k=1..n */
    for (idx_t k = 1; k <= n; ++k) {
        /* multiply by (n + k) */
        bigint_mul_uint(&binom, (uint32_t)(n + k));
        /* exact division by k */
        bigint_div_uint(&binom, (uint32_t)k);
    }

    /* divide by 2^(n+2) – we perform n+2 times integer division by 2. */
    for (idx_t i = 0; i < n + 2; ++i) {
        bigint_div_uint(&binom, 2U);
    }

    /* convert back to seq_t */
    seq_t result = bigint_to_seq_t(&binom);
    bigint_free(&binom);
    return result;
}
