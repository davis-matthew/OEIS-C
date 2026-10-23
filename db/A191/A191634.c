/*
 * A191634  a(n) = floor((1 + 3^n)/(1 + 3*n)).
 * https://oeis.org/A191634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((1 + 3^n)/(1 + 3*n))
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 44, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Big integer in base 1e9 (little‑endian) */
#define BI_BASE 1000000000U

typedef struct {
    uint32_t *d;   /* digits, least significant first */
    size_t len;    /* number of used digits */
} BigInt;

/* Initialize a BigInt to the value 1 */
static void bi_from_one(BigInt *bi) {
    bi->len = 1;
    bi->d   = (uint32_t *)malloc(bi->len * sizeof(uint32_t));
    bi->d[0] = 1;
}

/* Free a BigInt */
static void bi_free(BigInt *bi) {
    free(bi->d);
    bi->d = NULL;
    bi->len = 0;
}

/* Multiply BigInt by 3 */
static void bi_mul3(BigInt *bi) {
    uint64_t carry = 0;
    for (size_t i = 0; i < bi->len; ++i) {
        uint64_t prod = (uint64_t)bi->d[i] * 3 + carry;
        bi->d[i] = (uint32_t)(prod % BI_BASE);
        carry    = prod / BI_BASE;
    }
    if (carry) {
        bi->d = (uint32_t *)realloc(bi->d, (bi->len + 1) * sizeof(uint32_t));
        bi->d[bi->len] = (uint32_t)carry;
        bi->len++;
    }
}

/* Add 1 to BigInt */
static void bi_add1(BigInt *bi) {
    uint64_t carry = 1;
    for (size_t i = 0; i < bi->len && carry; ++i) {
        uint64_t sum = (uint64_t)bi->d[i] + carry;
        bi->d[i] = (uint32_t)(sum % BI_BASE);
        carry    = sum / BI_BASE;
    }
    if (carry) {
        bi->d = (uint32_t *)realloc(bi->d, (bi->len + 1) * sizeof(uint32_t));
        bi->d[bi->len] = (uint32_t)carry;
        bi->len++;
    }
}

/* Divide BigInt by small uint32 divisor:   bi / div = q  (q overwritten)   */
static void bi_div_small(BigInt *bi, uint32_t div, BigInt *q) {
    q->len = bi->len;
    q->d   = (uint32_t *)malloc(q->len * sizeof(uint32_t));
    uint64_t rem = 0;
    for (size_t i = bi->len; i-- > 0; ) {
        uint64_t cur = rem * BI_BASE + bi->d[i];
        q->d[i] = (uint32_t)(cur / div);
        rem     = cur % div;
    }
    /* normalize: remove leading zeros */
    while (q->len > 1 && q->d[q->len - 1] == 0)
        q->len--;
}

/* Convert BigInt to seq_t (assumes value fits) */
static seq_t bi_to_seqt(const BigInt *bi) {
    seq_t res = 0;
    for (size_t i = 0; i < bi->len; ++i) {
        /* process from most significant to least */
        res = res * BI_BASE + (seq_t)bi->d[bi->len - 1 - i];
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Compute 3^n + 1 as a big integer */
    BigInt bi;
    bi_from_one(&bi);
    for (idx_t i = 0; i < n; ++i)
        bi_mul3(&bi);
    bi_add1(&bi);

    /* Denominator: 1 + 3*n  (fits in 32 bits for n <= 10000) */
    uint32_t denom = (uint32_t)(3 * n + 1);

    /* Divide numerator by denominator */
    BigInt q;
    bi_div_small(&bi, denom, &q);

    /* Convert quotient to seq_t */
    seq_t result = bi_to_seqt(&q);

    /* Clean up */
    bi_free(&bi);
    bi_free(&q);

    return result;
}
