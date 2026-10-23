/*
 * A020108  Ceiling of Gamma(n+1/10)/Gamma(1/10).
 * https://oeis.org/A020108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil( Γ(n+1/10) / Γ(1/10) ) = ceil( ∏_{k=0}^{n-1} (10
 *            k+1) / 10ⁿ )
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Base for the internal representation (10⁹) */
#define BASE 1000000000U

/* 10⁰ … 10⁹ */
static const uint32_t POW10_10[10] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000
};

/* Small big‑integer type (least‐significant word first). */
typedef struct {
    uint32_t *d;   /* digit array (base 10⁹) */
    size_t   len;  /* number of used words */
    size_t   cap;  /* allocated capacity in words */
} BigInt;

/* Allocate initial big integer value 1 */
static void big_init(BigInt *x)
{
    x->len = 1;
    x->cap = 8;
    x->d   = (uint32_t *)malloc(x->cap * sizeof(uint32_t));
    x->d[0] = 1;
}

/* Free the allocated array */
static void big_free(BigInt *x)
{
    free(x->d);
}

/* Ensure capacity and zero‑initialize any new words */
static void big_grow(BigInt *x, size_t new_cap)
{
    if (new_cap <= x->cap) return;
    size_t old_cap = x->cap;
    x->cap = new_cap;
    x->d   = (uint32_t *)realloc(x->d, x->cap * sizeof(uint32_t));
    /* Zero new words to avoid garbage data */
    for (size_t i = old_cap; i < x->cap; ++i) x->d[i] = 0;
}

/* Multiply by a small 32‑bit integer */
static void big_mul_small(BigInt *x, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t prod = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)(prod % BASE);
        carry   = prod / BASE;
    }
    while (carry) {
        if (x->len == x->cap) big_grow(x, x->cap * 2);
        x->d[x->len++] = (uint32_t)(carry % BASE);
        carry /= BASE;
    }
}

/* Subtract one from the integer (assumes the result stays non‑negative) */
static void big_sub_one(BigInt *x)
{
    size_t i = 0;
    if (x->d[0] > 0) {
        x->d[0]--;
    } else {
        x->d[0] = BASE - 1;
        i = 1;
        while (i < x->len && x->d[i] == 0) {
            x->d[i++] = BASE - 1;
        }
        if (i < x->len) x->d[i]--;
    }
    /* Trim leading zeros */
    while (x->len > 1 && x->d[x->len - 1] == 0) --x->len;
}

/* Add 10ⁿ to the big integer (correct carry handling) */
static void big_add_pow10(BigInt *x, int n)
{
    int q = n / 9;
    int r = n % 9;
    uint32_t add = POW10_10[r];

    /* Ensure the array contains word q */
    if ((size_t)q >= x->len) {
        size_t old_len = x->len;
        big_grow(x, q + 1);
        for (size_t i = old_len; i < (size_t)q + 1; ++i) x->d[i] = 0;
        x->len = q + 1;
    }

    uint64_t sum = (uint64_t)x->d[q] + add;
    x->d[q] = (uint32_t)(sum % BASE);
    uint32_t carry = (uint32_t)(sum / BASE);
    size_t pos = q + 1;
    while (carry) {
        if (pos >= x->len) {
            size_t old_len = x->len;
            big_grow(x, pos + 1);
            for (size_t i = old_len; i < pos + 1; ++i) x->d[i] = 0;
            x->len = pos + 1;
        }
        sum = (uint64_t)x->d[pos] + carry;
        x->d[pos] = (uint32_t)(sum % BASE);
        carry = (uint32_t)(sum / BASE);
        ++pos;
    }
    if (pos > x->len) x->len = pos;
}

/* Divide the big integer by 10ⁿ (exact integer division) */
static void big_div_pow10(BigInt *x, int n)
{
    int q = n / 9;
    int r = n % 9;

    if (q >= (int)x->len) {
        x->len = 1;
        x->d[0] = 0;
        return;
    }

    if (q > 0) {
        /* Shift the array down by q words */
        size_t newlen = x->len - q;
        memmove(x->d, x->d + q, newlen * sizeof(uint32_t));
        x->len = newlen;
    }

    if (r > 0) {
        /* Full division by 10ʳ using small divisor algorithm */
        uint64_t rem = 0;
        for (int i = (int)x->len - 1; i >= 0; --i) {
            uint64_t cur = rem * BASE + x->d[i];
            x->d[i] = (uint32_t)(cur / POW10_10[r]);
            rem     = cur % POW10_10[r];
        }
        while (x->len > 1 && x->d[x->len - 1] == 0) --x->len;
    }
}

/* Convert the big integer to seq_t (assumes seq_t supports +, *, [[uint32_t]] operator) */
static seq_t big_to_seq(const BigInt *x)
{
    seq_t result = 0;
    for (int i = (int)x->len - 1; i >= 0; --i) {
        result = result * (seq_t)BASE + (seq_t)x->d[i];
    }
    return result;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* Γ(1/10)/Γ(1/10) = 1 */

    BigInt prod;
    big_init(&prod);

    /* Compute Π_{k=0}^{n-1} (10*k+1)  */
    for (idx_t k = 0; k < n; ++k) {
        uint32_t factor = (uint32_t)(10ULL * k + 1ULL);
        big_mul_small(&prod, factor);
    }

    /* Compute (Π + 10ⁿ – 1) / 10ⁿ  =  ceil(Π / 10ⁿ) */
    big_add_pow10(&prod, (int)n);   /* add 10ⁿ */
    big_sub_one(&prod);             /* subtract 1 */
    big_div_pow10(&prod, (int)n);   /* divide by 10ⁿ */

    seq_t result = big_to_seq(&prod);
    big_free(&prod);
    return result;
}
