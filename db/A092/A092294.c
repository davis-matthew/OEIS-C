/*
 * A092294  Decimal expansion of 3 + sqrt(15).
 * https://oeis.org/A092294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, cons
 * Formula:   a(n) = the n‑th decimal digit (1‑indexed) of 3 + sqrt(15) (a(1)
 *            is the integer part, then successive decimal digits)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Small big integer type (base 1 000 000 000).                            */
/* --------------------------------------------------------------------- */
#define BIGBASE 1000000000U

typedef struct {
    uint32_t *d;   /* little‑endian digits, d[0] least significant */
    size_t   len;  /* number of used digits */
    size_t   cap;  /* allocated digits */
} BigInt;

static void bigensure(BigInt *x, size_t needed)
{
    if (x->cap < needed) {
        size_t newcap = x->cap ? x->cap * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(x->d, newcap * sizeof(uint32_t));
        x->d = tmp;
        x->cap = newcap;
    }
}

static void bignull(BigInt *x)
{   x->len = 0; }

static void bigset_uint32(BigInt *x, uint32_t v)
{
    if (v == 0) { bignull(x); return; }
    bigensure(x, 1);
    x->len = 1;
    x->d[0] = v;
}

static void bigcopy(BigInt *dst, const BigInt *src)
{
    bigensure(dst, src->len);
    dst->len = src->len;
    for (size_t i = 0; i < src->len; ++i) dst->d[i] = src->d[i];
}

static void bigmul_small(BigInt *x, uint32_t m)
{
    if (m == 0) { bignull(x); return; }
    if (x->len == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BIGBASE);
        carry = prod / BIGBASE;
    }
    if (carry) {
        bigensure(x, x->len + 1);
        x->d[x->len++] = (uint32_t)carry;
    }
}

static void bigadd_small(BigInt *x, uint32_t m)
{
    if (m == 0) return;
    uint64_t carry = m;
    size_t i = 0;
    for (; i < x->len; ++i) {
        uint64_t sum = (uint64_t)x->d[i] + carry;
        x->d[i] = (uint32_t)(sum % BIGBASE);
        carry = sum / BIGBASE;
        if (carry == 0) break;
    }
    while (carry) {
        bigensure(x, x->len + 1);
        x->d[x->len++] = (uint32_t)carry;
        carry /= BIGBASE;
    }
}

static int bigcmp(const BigInt *a, const BigInt *b)
{
    if (a->len > b->len) return 1;
    if (a->len < b->len) return -1;
    for (long long i = (long long)a->len - 1; i >= 0; --i) {
        if (a->d[i] > b->d[i]) return 1;
        if (a->d[i] < b->d[i]) return -1;
    }
    return 0;
}

static void bigsub(BigInt *a, const BigInt *b)   /* a -= b, a >= b */
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t bi = (i < b->len) ? b->d[i] : 0;
        uint64_t ai = a->d[i];
        uint64_t diff = ai - bi - borrow;
        if (diff >> 63) {                /* was negative */
            a->d[i] = (uint32_t)(diff + BIGBASE);
            borrow = 1;
        } else {
            a->d[i] = (uint32_t)diff;
            borrow = 0;
        }
    }
    while (a->len > 0 && a->d[a->len - 1] == 0) --a->len;
}

/* --------------------------------------------------------------------- */
/* Cache for already computed digits.                                    */
/* --------------------------------------------------------------------- */
static uint8_t *digit_cache = NULL;
static size_t   digit_len   = 0;
static size_t   digit_cap   = 0;

static void ensure_digits(size_t needed)
{
    if (digit_cap < needed) {
        size_t newcap = digit_cap ? digit_cap * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        uint8_t *newarr = (uint8_t *)realloc(digit_cache, newcap);
        digit_cache = newarr;
        digit_cap   = newcap;
    }
}

/* --------------------------------------------------------------------- */
/* Main function                                                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* undefined for n==0, but safe */

    /* static state for the digit‑by‑digit sqrt algorithm */
    static BigInt root   = {NULL,0,0};
    static BigInt rem    = {NULL,0,0};
    static BigInt t      = {NULL,0,0};
    static BigInt prod   = {NULL,0,0};
    static BigInt temp   = {NULL,0,0};
    static int initialized = 0;

    if (!initialized) {
        /* first digit 6 (integer part of 3 + sqrt(15)) */
        ensure_digits(1);
        digit_cache[0] = 6;
        digit_len = 1;

        /* initialise the sqrt(15) routine */
        bigset_uint32(&root, 3);   /* floor(sqrt(15)) = 3          */
        bigset_uint32(&rem, 6);    /* remainder 15 - 3^2 = 6       */

        initialized = 1;
    }

    /* generate digits until we have at least n of them */
    while (digit_len < (size_t)n) {
        /* bring down next pair of zeros : remainder *= 100 */
        bigmul_small(&rem, 100);

        /* t = root * 20 */
        bigcopy(&t, &root);
        bigmul_small(&t, 20);

        int d = 0;
        for (int dd = 9; dd >= 0; --dd) {
            /* prod = (t + dd) * dd */
            bigcopy(&temp, &t);
            bigadd_small(&temp, (uint32_t)dd);   /* temp = t + dd   */
            bigcopy(&prod, &temp);
            bigmul_small(&prod, (uint32_t)dd);   /* prod  = (t+dd)*dd */

            if (bigcmp(&prod, &rem) <= 0) {
                d = dd;
                bigsub(&rem, &prod);             /* rem -= prod      */
                bigmul_small(&root, 10);          /* root = root*10   */
                bigadd_small(&root, (uint32_t)d); /* root += d        */
                break;
            }
        }

        /* cache the new digit */
        ensure_digits(digit_len + 1);
        digit_cache[digit_len++] = (uint8_t)d;
    }

    return (seq_t)(digit_cache[n - 1]);
}
