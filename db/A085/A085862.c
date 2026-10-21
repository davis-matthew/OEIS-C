/*
 * A085862  Number of 8's in decimal expansion of Fibonacci(n).
 * https://oeis.org/A085862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of decimal digits equal to 8 in Fibonacci(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is self‑contained, uses an arbitrary‑precision integer
 * representation based on base 1 000 000 000.  It works for any seq_t
 * (unsigned integer or arbitrary precision) and scales to n≈10000. */

#include <stdlib.h>
#include <stdio.h>

/* Big integer in base 1e9 ---------------------------------------------------- */
#define BIG_BASE 1000000000U        /* 1e9 < 2^32, fits in uint32_t           */
#define BIG_MAX_CAP 300             /* fits Fibonacci(10000) (~2089 digits)   */

typedef struct {
    uint32_t limbs[BIG_MAX_CAP];
    size_t   len;         /* number of used limbs */
} BigInt;

/* initialise a BigInt with a small integer value */
static void big_init(BigInt *b, uint32_t v) {
    b->len = 1;
    b->limbs[0] = v;
}

/* add two BigInts: r = x + y */
static void big_add(const BigInt *x, const BigInt *y, BigInt *r) {
    const size_t maxlen = (x->len > y->len) ? x->len : y->len;
    size_t i;
    uint64_t carry = 0;
    for (i = 0; i < maxlen; ++i) {
        uint64_t a = (i < x->len) ? x->limbs[i] : 0;
        uint64_t b = (i < y->len) ? y->limbs[i] : 0;
        uint64_t sum = a + b + carry;
        r->limbs[i] = (uint32_t)(sum % BIG_BASE);
        carry = sum / BIG_BASE;
    }
    if (carry) {
        r->limbs[i++] = (uint32_t)carry;
    }
    r->len = i;
}

/* count digit '8' in the decimal representation of a BigInt */
static uint64_t big_count_eights(const BigInt *b) {
    uint64_t count = 0;
    char buf[10];                 /* enough for "%09u" and null byte */
    size_t i;
    /* most significant limb (no leading zeros) */
    if (b->len > 0) {
        snprintf(buf, sizeof(buf), "%u", b->limbs[b->len - 1]);
        for (i = 0; buf[i]; ++i)
            if (buf[i] == '8') ++count;
    }
    /* remaining limbs (always 9 digits, padded with zeros) */
    for (i = b->len - 1; i-- > 0; ) {
        snprintf(buf, sizeof(buf), "%09u", b->limbs[i]);
        for (size_t j = 0; buf[j]; ++j)
            if (buf[j] == '8') ++count;
    }
    return count;
}

/* ------------------------------------------------------------------------ */
/* Public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* handle first two Fibonacci numbers */
    if (n <= 1) return (seq_t)0;

    /* BigInt instances for F(n-2), F(n-1) and result */
    BigInt f_prev, f_curr, f_next;

    big_init(&f_prev, 0);            /* F(0) */
    big_init(&f_curr, 1);            /* F(1) */

    /* iteratively compute Fibonacci numbers up to F(n) */
    for (idx_t i = 2; i <= n; ++i) {
        big_add(&f_prev, &f_curr, &f_next);  /* f_next = f_prev + f_curr */
        /* shift for next iteration */
        f_prev.limbs[0] = f_curr.limbs[0];
        f_prev.len = f_curr.len;
        for (size_t k = 1; k < f_curr.len; ++k)
            f_prev.limbs[k] = f_curr.limbs[k];

        f_curr.limbs[0] = f_next.limbs[0];
        f_curr.len = f_next.len;
        for (size_t k = 1; k < f_next.len; ++k)
            f_curr.limbs[k] = f_next.limbs[k];
    }

    /* f_curr now holds F(n) */
    uint64_t eights = big_count_eights(&f_curr);
    return (seq_t)eights;
}
