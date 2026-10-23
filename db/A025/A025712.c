/*
 * A025712  Index of 6^n within the sequence of the numbers of the form
 *          2^i*6^j.
 * https://oeis.org/A025712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Σ_{k=1..n} (k + bitlen(3^k))
 * Range:     u8 n <= 13, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where bitlen(x) is the number of bits of the positive integer x */
#include <stdint.h>
#include <stdlib.h>

/* Simple small 32‑bit limb big integer, only multiplication by 3 is required */
typedef struct {
    uint32_t *limb;   /* little‑endian limbs, limb[0] is least significant */
    size_t    len;    /* number of limbs */
} BigInt;

static void bigint_init(BigInt *bi)
{
    bi->len  = 1;
    bi->limb = (uint32_t *)malloc(sizeof(uint32_t));
    bi->limb[0] = 1;          /* start with value 1 (3^0) */
}

static void bigint_free(BigInt *bi)
{
    free(bi->limb);
}

static void bigint_mul3(BigInt *bi)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < bi->len; i++) {
        uint64_t prod = (uint64_t)bi->limb[i] * 3 + carry;
        bi->limb[i]   = (uint32_t)prod;
        carry         = prod >> 32;
    }
    if (carry) {
        bi->limb = (uint32_t *)realloc(bi->limb, sizeof(uint32_t) * (bi->len + 1));
        bi->limb[bi->len++] = (uint32_t)carry;
    }
}

static inline int bitlen_uint32(uint32_t x)
{
    int cnt = 0;
    while (x) {
        x >>= 1;
        cnt++;
    }
    return cnt;
}

/* bit length of a positive big integer */
static int bitlen_bigint(const BigInt *bi)
{
    uint32_t msb = bi->limb[bi->len - 1];
    int       lb  = bitlen_uint32(msb);
    return (int)(32 * (bi->len - 1) + lb);
}

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    BigInt power;
    bigint_init(&power);          /* power = 3^0 = 1 */
    seq_t sum = 0;                /* sum of (k + bitlen(3^k)) for k=1..n */

    for (idx_t k = 1; k <= n; ++k) {
        bigint_mul3(&power);                 /* power = 3^k */
        int bl = bitlen_bigint(&power);      /* bitlen(3^k) */
        seq_t ceil_kc = (seq_t)k + (seq_t)bl;/* ceil(k*log2(6)) as derived */
        sum += ceil_kc;
    }

    bigint_free(&power);
    return (seq_t)1 + sum;
}
