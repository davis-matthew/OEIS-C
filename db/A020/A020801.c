/*
 * A020801  Decimal expansion of 1/sqrt(44).
 * https://oeis.org/A020801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the nth decimal digit of 1/sqrt(44).
 *  The digit a(n) is the (n+1)th digit after the decimal point of
 *  1/sqrt(44), i.e. a(n) = floor(10^(n+1) / sqrt(44)) mod 10.
 *  Implementation uses only integer arithmetic and an internal
 *  small big‑integer representation sufficient for the required size.
 */

#include <stdint.h>
#include <stdlib.h>

/* ----- Small fixed‑base big integer (base 1e9) ----- */
#define BI_BASE        1000000000U   /* < 2^32 */
#define BI_BASE_MOD    120U          /* BI_BASE % 440 */

typedef struct {
    uint32_t *d;  /* least significant limb first */
    size_t   len; /* number of used limbs */
    size_t   cap; /* allocated capacity */
} BigInt;

/* Constructors & destructors */
static void bi_init(BigInt *a) { a->d = NULL; a->len = a->cap = 0; }
static void bi_free(BigInt *a) { free(a->d); a->d = NULL; a->len = a->cap = 0; }

/* Allocate or shrink to exactly one limb containing v (v>0). */
static void bi_set_uint(BigInt *a, uint32_t v)
{
    if (v == 0) { bi_free(a); return; }
    if (a->cap < 1) {
        a->d = (uint32_t *)realloc(a->d, sizeof(uint32_t));
        a->cap = 1;
    }
    a->d[0] = v;
    a->len = 1;
}

/* Copy src into dst. */
static void bi_copy(BigInt *dst, const BigInt *src)
{
    if (dst == src) return;
    if (src->len == 0) { bi_free(dst); return; }
    if (dst->cap < src->len) {
        dst->d = (uint32_t *)realloc(dst->d, src->len * sizeof(uint32_t));
        dst->cap = src->len;
    }
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
    dst->len = src->len;
}

/* Multiply instant BigInt a by small unsigned 32‑bit m. */
static void bi_mul_small(BigInt *a, uint32_t mul)
{
    if (mul == 0) { bi_free(a); return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * mul + carry;
        a->d[i] = (uint32_t)(prod % BI_BASE);
        carry   = prod / BI_BASE;
    }
    if (carry) {
        if (a->cap <= a->len) {
            a->d = (uint32_t *)realloc(a->d, (++a->cap) * sizeof(uint32_t));
        }
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* Add small unsigned 32‑bit a to BigInt a, a += add. */
static void bi_add_small(BigInt *a, uint32_t add)
{
    uint64_t sum = (uint64_t)(a->len ? a->d[0] : 0) + add;
    if (!a->len) { a->d = (uint32_t *)realloc(a->d, sizeof(uint32_t)); a->len = 1; }
    a->d[0] = (uint32_t)(sum % BI_BASE);
    uint64_t carry = sum / BI_BASE;
    size_t i = 1;
    while (carry && i < a->len) {
        uint64_t s = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(s % BI_BASE);
        carry   = s / BI_BASE;
        ++i;
    }
    if (carry) {
        if (a->cap <= a->len) {
            a->d = (uint32_t *)realloc(a->d, (++a->cap) * sizeof(uint32_t));
        }
        a->d[a->len++] = (uint32_t)carry;
    }
}

/* Subtract BigInt b from a (assumes a >= b). */
static void bi_sub(BigInt *a, const BigInt *b)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t val = (int64_t)(i < b->len ? b->d[i] : 0);
        int64_t diff = (int64_t)a->d[i] - val - borrow;
        if (diff < 0) { diff += BI_BASE; borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (a->len && a->d[a->len-1] == 0) --a->len;
}

/* Compare two BigInts: returns 1 if a>b, -1 if a<b, 0 if equal. */
static int bi_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

/* ----- Sequence function ----- */
seq_t a(idx_t n)
{
    /* Basic algorithm:
     *   I = floor( sqrt(44) * 10^(n+1) )
     *   a(n) = (I mod 440) / 44
     */
    idx_t K = n + 1;  /* number of decimal places needed */

    BigInt root, remainder, root20, cand;
    bi_init(&root); bi_init(&remainder); bi_init(&root20); bi_init(&cand);

    /* Initial value: floor(sqrt(44)) = 6.  remainder = 44 - 6^2 = 8 */
    uint32_t d0 = 0;
    for (uint32_t d = 9; d-- > 0; ) if (d*d <= 44) { d0 = d; break; }
    bi_set_uint(&root, d0);
    bi_set_uint(&remainder, (uint32_t)(44 - d0*d0));

    for (idx_t cnt = 0; cnt < K; ++cnt) {
        /* Bring down next two decimal zeros */
        bi_mul_small(&remainder, 100);

        /* Compute root20 = root * 20 */
        bi_copy(&root20, &root);
        bi_mul_small(&root20, 20);

        /* Find largest digit d so that (root20 + d) * d <= remainder */
        uint32_t d;
        for (d = 9; ; --d) {
            bi_copy(&cand, &root20);
            bi_add_small(&cand, d);
            bi_mul_small(&cand, d);
            if (bi_cmp(&cand, &remainder) <= 0) break;
        }

        /* Update root = root*10 + d */
        bi_mul_small(&root, 10);
        bi_add_small(&root, d);

        /* remainder -= cand */
        bi_sub(&remainder, &cand);
    }

    /* Compute I mod 440 efficiently from the big integer root */
    uint64_t mod = 0;
    for (size_t i = root.len; i-- > 0; ) {
        mod = (mod * BI_BASE_MOD + root.d[i]) % 440U;
    }
    seq_t digit = (seq_t)(mod / 44U);

    bi_free(&cand); bi_free(&root20); bi_free(&root); bi_free(&remainder);
    return digit;
}
