/*
 * A215633  Decimal expansion of Sum_{n>=1} 1/n^(n^prime(n)).
 * https://oeis.org/A215633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn
 * Formula:   a(n) = decimal digit at position n of S = 1 + sum_{k>=2}
 *            1/(k^{prime(k)}) (prime(k) = k-th prime)
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t *d;      /* little-endian limbs */
    size_t len;       /* number of limbs (len==0 means zero) */
} BigInt;

/* ------------ small big integer helpers ------------ */
static void bigint_set_uint32(BigInt *a, uint32_t v)
{
    if (v == 0) {
        a->len = 0;
        a->d = NULL;
    } else {
        a->len = 1;
        a->d   = (uint32_t *)malloc(sizeof(uint32_t));
        a->d[0] = v;
    }
}

static void bigint_mul_uint32(BigInt *a, uint32_t m)
{
    if (m == 1 || a->len == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod & 0xFFFFFFFF);
        carry   = prod >> 32;
    }
    while (carry) {
        a->d = (uint32_t *)realloc(a->d, (a->len + 1) * sizeof(uint32_t));
        a->d[a->len++] = (uint32_t)(carry & 0xFFFFFFFF);
        carry >>= 32;
    }
}

static void bigint_shift_left_one(BigInt *a)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t temp = ((uint64_t)a->d[i] << 1) | carry;
        a->d[i] = (uint32_t)temp;
        carry   = temp >> 32;
    }
    if (carry) {
        a->d = (uint32_t *)realloc(a->d, (a->len + 1) * sizeof(uint32_t));
        a->d[a->len++] = carry;
    }
}

static int bigint_cmp(const BigInt *a, const BigInt *b)
{
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (int i = (int)a->len - 1; i >= 0; --i) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

static void bigint_sub_inplace(BigInt *a, const BigInt *b)  /* assumes a >= b */
{
    int64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t ai = (int64_t)a->d[i];
        int64_t bi = (i < b->len) ? (int64_t)b->d[i] : 0;
        int64_t diff = ai - bi - borrow;
        if (diff < 0) {
            diff += (1LL << 32);
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    while (a->len > 0 && a->d[a->len - 1] == 0)
        --a->len;
}

static void bigint_free(BigInt *a)
{
    free(a->d);
    a->len = 0;
}

/* ------------ denominators 3^243 and 2^32768 ------------ */
static BigInt denom3, denom4;
static int denom3_ready = 0, denom4_ready = 0;

static void build_denom3(void)
{
    bigint_set_uint32(&denom3, 1);
    for (int i = 0; i < 243; ++i)
        bigint_mul_uint32(&denom3, 3);
    denom3_ready = 1;
}

static void build_denom4(void)
{
    bigint_set_uint32(&denom4, 1);
    for (int i = 0; i < 32768; ++i)
        bigint_shift_left_one(&denom4);
    denom4_ready = 1;
}

/* ------------ sequence computation ------------ */
seq_t a(idx_t n)
{
    static idx_t maxComputed = 0;
    static seq_t *sumDigits = NULL;
    static uint8_t *digits2 = NULL;
    static uint8_t *digits3 = NULL;
    static uint8_t *digits4 = NULL;

    if (n > maxComputed) {                /* increase precision   */
        idx_t N = n;
        sumDigits = (seq_t *)realloc(sumDigits, (N + 1) * sizeof(seq_t));
        memset(sumDigits, 0, (N + 1) * sizeof(seq_t));
        digits2 = (uint8_t *)realloc(digits2, (N + 1) * sizeof(uint8_t));
        memset(digits2, 0, (N + 1) * sizeof(uint8_t));
        digits3 = (uint8_t *)realloc(digits3, (N + 1) * sizeof(uint8_t));
        memset(digits3, 0, (N + 1) * sizeof(uint8_t));
        digits4 = (uint8_t *)realloc(digits4, (N + 1) * sizeof(uint8_t));
        memset(digits4, 0, (N + 1) * sizeof(uint8_t));

        if (!denom3_ready) build_denom3();
        if (!denom4_ready) build_denom4();

        /* term 1/256 (n=2)                                 */
        uint32_t rem2 = 1;
        digits2[1] = 0;            /* integer part */
        for (idx_t pos = 2; pos <= N; ++pos) {
            rem2 *= 10;
            digits2[pos] = rem2 / 256;
            rem2 %= 256;
        }

        /* term 1/3^243 (n=3)                               */
        BigInt rem3;
        bigint_set_uint32(&rem3, 1);
        digits3[1] = 0;
        for (idx_t pos = 2; pos <= N; ++pos) {
            bigint_mul_uint32(&rem3, 10);
            int digit = 0;
            while (bigint_cmp(&rem3, &denom3) >= 0) {
                bigint_sub_inplace(&rem3, &denom3);
                ++digit;
            }
            digits3[pos] = digit;
        }
        bigint_free(&rem3);

        /* term 1/2^32768 (n=4)                               */
        BigInt rem4;
        bigint_set_uint32(&rem4, 1);
        digits4[1] = 0;
        for (idx_t pos = 2; pos <= N; ++pos) {
            bigint_mul_uint32(&rem4, 10);
            int digit = 0;
            while (bigint_cmp(&rem4, &denom4) >= 0) {
                bigint_sub_inplace(&rem4, &denom4);
                ++digit;
            }
            digits4[pos] = digit;
        }
        bigint_free(&rem4);

        /* accumulate digits (integer part starts with 1)     */
        sumDigits[1] = 1;           /* integer part of constant */
        for (idx_t pos = 2; pos <= N; ++pos)
            sumDigits[pos] = (seq_t)digits2[pos] + digits3[pos] + digits4[pos];

        /* propagate carries from least to most significant  */
        int carry = 0;
        for (idx_t pos = N; pos >= 1; --pos) {
            int val = (int)sumDigits[pos] + carry;
            sumDigits[pos] = (seq_t)(val % 10);
            carry = val / 10;
        }

        maxComputed = N;
    }
    return sumDigits[n];
}
