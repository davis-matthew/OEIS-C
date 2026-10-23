/*
 * A020073  a(n) = floor( Gamma(n+1/8)/Gamma(1/8) ).
 * https://oeis.org/A020073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Gamma(n+1/8)/Gamma(1/8) )
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses arbitrary‑precision integer arithmetic
   internally. The result is converted to the build‑specific seq_t.   */

#include <stdint.h>
#include <stdlib.h>

/*** Big integer implementation used internally ***/
typedef struct {
    uint64_t *limb;   /* little‑endian limbs, LIMB[0] is the least significant */
    size_t   len;     /* number of used limbs */
    size_t   cap;     /* allocated capacity */
} BigInt;

static void bi_init(BigInt *x)
{
    x->cap = 8;
    x->limb = (uint64_t *)malloc(sizeof(uint64_t) * x->cap);
    x->len = 1;
    x->limb[0] = 1ULL;       /* start with value 1 */
}

static void bi_free(BigInt *x)
{
    free(x->limb);
}

static void bi_resize(BigInt *x, size_t newcap)
{
    if (newcap <= x->cap)
        return;
    uint64_t *newlimb = (uint64_t *)realloc(x->limb, sizeof(uint64_t) * newcap);
    if (newlimb) {
        x->limb = newlimb;
        x->cap = newcap;
    }
}

static void bi_mul_small(BigInt *x, uint64_t m)
{
    __uint128_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        __uint128_t prod = (__uint128_t)x->limb[i] * m + carry;
        x->limb[i] = (uint64_t)prod;
        carry = prod >> 64;
    }
    if (carry) {
        if (x->len == x->cap)
            bi_resize(x, x->cap * 2);
        x->limb[x->len++] = (uint64_t)carry;
    }
}

/* shift right by `shift` bits (shift > 0).  Result is truncated. */
static void bi_shift_right(BigInt *x, size_t shift)
{
    if (shift == 0)
        return;
    size_t whole = shift / 64;
    unsigned bits = shift % 64;

    if (whole >= x->len) {               /* shift exceeds the number of limbs */
        for (size_t i = 0; i < x->len; ++i)
            x->limb[i] = 0;
        x->len = 1;
        return;
    }

    size_t newlen = x->len - whole;
    if (bits == 0) {
        for (size_t i = 0; i < newlen; ++i)
            x->limb[i] = x->limb[i + whole];
    } else {
        for (size_t i = 0; i < newlen - 1; ++i) {
            x->limb[i] = (x->limb[i + whole] >> bits) |
                         (x->limb[i + whole + 1] << (64 - bits));
        }
        x->limb[newlen - 1] = x->limb[newlen + whole - 1] >> bits;
    }
    x->len = newlen;

    /* trim leading zeros */
    while (x->len > 1 && x->limb[x->len - 1] == 0)
        --x->len;
}

/* Convert BigInt to seq_t.  Handles build‑specific size of seq_t. */
static seq_t bi_to_seq(const BigInt *x)
{
    if (sizeof(seq_t) <= 8) {            /* small seq_t: only the lowest limb is used */
        return (seq_t)(x->limb[0]);
    } else {                             /* arbitrary precision seq_t: rebuild from limbs */
        seq_t result = 0;
        for (int i = (int)x->len - 1; i >= 0; --i) {
            result <<= 64;                /* seq_t must support bit shift */
            result += (seq_t)(x->limb[i]);
        }
        return result;
    }
}

/*** Public interface ***/
seq_t a(idx_t n)
{
    BigInt bi;
    bi_init(&bi);

    for (idx_t k = 0; k < n; ++k) {
        uint64_t mult = 8ULL * k + 1ULL; /* (8k+1) */
        bi_mul_small(&bi, mult);
    }

    /* divide by 8^n  →  shift right by 3n bits */
    bi_shift_right(&bi, (size_t)(3ULL * n));

    seq_t result = bi_to_seq(&bi);
    bi_free(&bi);
    return result;
}
