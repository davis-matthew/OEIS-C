/*
 * A017988  Powers of cube root of 5 rounded down.
 * https://oeis.org/A017988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(5^(n/3))
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 41, u64 n <= 82, u128 n <= 165,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../utils/seq_from_str.h"

#define BI_BASE 1000000000U       /* 1e9, fits in 32 bits */

typedef struct Bi {
    uint32_t *d; /* little endian limbs, base 1e9 */
    size_t len;  /* number of limbs */
} Bi;

/* forward declarations */
static Bi *bi_new(size_t len);
static void bi_free(Bi *a);
static Bi *bi_from_uint64(uint64_t v);
static int bi_cmp(const Bi *a, const Bi *b);   /* >0 if a>b, <0 if a<b, 0 equal */
static Bi *bi_add(const Bi *a, const Bi *b);
static Bi *bi_sub(const Bi *a, const Bi *b);   /* assumes a>=b */
static Bi *bi_mul(const Bi *a, const Bi *b);
static Bi *bi_div2(const Bi *a);
static Bi *bi_pow_uint64(uint64_t base, idx_t exp);
static char *bi_to_string(const Bi *a);
static Bi *bi_cube_root_floor(const Bi *x);

/* -------------------------------------------------------------------------- */
/* Basic BigInt functions                                                 */
/* -------------------------------------------------------------------------- */

static Bi *bi_new(size_t len)
{
    Bi *r = (Bi *)malloc(sizeof(Bi));
    r->len = len;
    r->d  = (uint32_t *)calloc(len, sizeof(uint32_t));
    return r;
}

static void bi_free(Bi *a)
{
    if (!a) return;
    free(a->d);
    free(a);
}

static Bi *bi_from_uint64(uint64_t v)
{
    if (v == 0) return bi_new(1);
    size_t len = 0;
    uint64_t tmp = v;
    while (tmp) { tmp /= BI_BASE; len++; }
    Bi *r = bi_new(len);
    tmp = v;
    for (size_t i = 0; i < len; ++i) { r->d[i] = tmp % BI_BASE; tmp /= BI_BASE; }
    return r;
}

static int bi_cmp(const Bi *a, const Bi *b)
{
    if (a->len != b->len) return (a->len > b->len) ? 1 : -1;
    for (size_t i = a->len; i-- > 0; ) {
        if (a->d[i] != b->d[i]) return (a->d[i] > b->d[i]) ? 1 : -1;
    }
    return 0;
}

static Bi *bi_add(const Bi *a, const Bi *b)
{
    size_t n = (a->len > b->len) ? a->len : b->len;
    Bi *r = bi_new(n + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < n; ++i) {
        uint64_t sum = carry;
        if (i < a->len) sum += a->d[i];
        if (i < b->len) sum += b->d[i];
        r->d[i] = (uint32_t)(sum % BI_BASE);
        carry   = sum / BI_BASE;
    }
    r->d[n] = (uint32_t)carry;
    if (r->d[n] == 0) { r->len--; }
    return r;
}

static Bi *bi_sub(const Bi *a, const Bi *b)
{
    /* assumes a >= b */
    Bi *r = bi_new(a->len);
    int64_t carry = 0;
    for (size_t i = 0; i < a->len; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->len ? b->d[i] : 0) - carry;
        if (diff < 0) { diff += BI_BASE; carry = 1; } else { carry = 0; }
        r->d[i] = (uint32_t)diff;
    }
    while (r->len > 1 && r->d[r->len-1] == 0) r->len--;
    return r;
}

static Bi *bi_mul(const Bi *a, const Bi *b)
{
    Bi *r = bi_new(a->len + b->len);
    for (size_t i = 0; i < a->len; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < b->len; ++j) {
            uint64_t cur = (uint64_t)a->d[i] * b->d[j] + r->d[i+j] + carry;
            r->d[i+j] = (uint32_t)(cur % BI_BASE);
            carry     = cur / BI_BASE;
        }
        size_t pos = i + b->len;
        while (carry) {
            uint64_t cur = (uint64_t)r->d[pos] + carry;
            r->d[pos] = (uint32_t)(cur % BI_BASE);
            carry     = cur / BI_BASE;
            ++pos;
        }
    }
    while (r->len > 1 && r->d[r->len-1] == 0) r->len--;
    return r;
}

static Bi *bi_div2(const Bi *a)
{
    Bi *r = bi_new(a->len);
    uint64_t carry = 0;
    for (size_t i = a->len; i-- > 0; ) {
        uint64_t cur = a->d[i] + carry * BI_BASE;
        r->d[i] = (uint32_t)(cur >> 1);   /* divide by 2 */
        carry   = cur & 1;                /* remainder */
    }
    while (r->len > 1 && r->d[r->len-1] == 0) r->len--;
    return r;
}

static Bi *bi_pow_uint64(uint64_t base, idx_t exp)
{
    Bi *result = bi_from_uint64(1);
    Bi *b      = bi_from_uint64(base);
    while (exp) {
        if (exp & 1) {
            Bi *tmp = bi_mul(result, b);
            bi_free(result);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {
            Bi *tmp = bi_mul(b, b);
            bi_free(b);
            b = tmp;
        }
    }
    bi_free(b);
    return result;
}

/* -------------------------------------------------------------------------- */
/*  Convert a BigInt to decimal string                                      */
/* -------------------------------------------------------------------------- */

static char *bi_to_string(const Bi *a)
{
    if (a->len == 1 && a->d[0] == 0)
        return strdup("0");

    int total_digits = (int)((a->len - 1) * 9);
    uint32_t ms = a->d[a->len - 1];
    while (ms) { total_digits++; ms /= 10; }
    char *buf = (char *)malloc(total_digits + 1);
    char *p = buf;
    /* most significant limb */
    p += sprintf(p, "%u", a->d[a->len - 1]);
    /* remaining limbs, padded to 9 digits */
    for (int i = (int)a->len - 2; i >= 0; --i) {
        p += sprintf(p, "%09u", a->d[i]);
    }
    *p = '\0';
    return buf;
}

/* -------------------------------------------------------------------------- */
/*  Integer cube root by binary search                                       */
/* -------------------------------------------------------------------------- */

static Bi *bi_cube_root_floor(const Bi *x)
{
    /* estimate decimal digits */
    int digits_dec = (int)((x->len - 1) * 9);
    uint32_t ms    = x->d[x->len - 1];
    while (ms) { digits_dec++; ms /= 10; }
    int high_exp = (digits_dec + 2) / 3;   /* ceil(digits/3) */

    Bi *low  = bi_from_uint64(0);
    Bi *high = bi_pow_uint64(10, high_exp);

    while (bi_cmp(low, high) < 0) {
        Bi *sum1 = bi_add(low, high);
        Bi *sum  = bi_add(sum1, bi_from_uint64(1));
        free(sum1);
        Bi *mid = bi_div2(sum);
        free(sum);

        /* mid^3 */
        Bi *mid_sq = bi_mul(mid, mid);
        Bi *mid_cu = bi_mul(mid_sq, mid);
        bi_free(mid_sq);

        int cmp_res = bi_cmp(mid_cu, x);
        if (cmp_res <= 0) {          /* mid^3 <= x */
            bi_free(low);
            low = mid;
            bi_free(mid_cu);
        } else {                     /* mid^3 > x */
            Bi *prev = bi_sub(mid, bi_from_uint64(1));
            bi_free(mid);
            bi_free(mid_cu);
            bi_free(high);
            high = prev;
        }
    }

    bi_free(high);
    return low;      /* low is floor cube root */
}

/* -------------------------------------------------------------------------- */
/*  Main function returning seq_t                                            */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* compute 5^n as big integer */
    Bi *pow5 = bi_pow_uint64(5, n);

    /* floor cube root of pow5 */
    Bi *root = bi_cube_root_floor(pow5);

    /* convert to string */
    char *str = bi_to_string(root);

    /* convert to seq_t */
    seq_t result = seq_from_str(str);

    /* clean up */
    free(str);
    bi_free(root);
    bi_free(pow5);

    return result;
}
