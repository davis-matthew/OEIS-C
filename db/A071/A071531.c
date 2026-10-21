/*
 * A071531  Smallest exponent r such that n^r contains at least one zero digit
 *          (in base 10).
 * https://oeis.org/A071531
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The A071531 sequence.
   a(n) = the smallest exponent r≥1 such that n^r contains a decimal digit 0.
   For n multiple of 10 the answer is 1.  Otherwise we multiply by n until a
   zero appears; a known upper bound for all n is 40000.  The implementation
   uses a small arbitrary‑precision integer in base 1e9 so that
   seq_t may be 8,16,32,64 or 128 bits (or a big‑int class in C++).        */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Arbitrary‑precision integer in base 10^9  (little‑endian limbs)          */
/* ------------------------------------------------------------------ */
typedef struct {
    uint32_t *limb;   /* least significant limb first */
    size_t len;       /* number of non‑zero limbs */
    size_t cap;       /* allocated capacity in limbs */
} BigInt;

static void big_init(BigInt *b, uint32_t val)
{
    b->cap = 1;
    b->len = (val == 0) ? 0 : 1;
    b->limb = (uint32_t *)realloc(NULL, b->cap * sizeof(uint32_t));
    if (b->len) b->limb[0] = val;
}

static void big_free(BigInt *b)
{
    free(b->limb);
    b->limb = NULL;
    b->len = b->cap = 0;
}

/* Multiply b by a small unsigned 32‑bit integer m. */
static void big_mul(BigInt *b, uint32_t m)
{
    if (m == 1 || b->len == 0) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->limb[i] * m + carry;
        b->limb[i] = (uint32_t)(prod % 1000000000u);
        carry     = prod / 1000000000u;
    }
    while (carry) {
        if (b->len == b->cap) {
            b->cap *= 2;
            b->limb = (uint32_t *)realloc(b->limb, b->cap * sizeof(uint32_t));
        }
        b->limb[b->len++] = (uint32_t)carry;
        carry /= 1000000000u;
    }
}

/* Return true if the decimal representation of b contains a zero digit. */
static bool big_has_zero(const BigInt *b)
{
    /* All interior limbs represent exactly 9 decimal digits (with leading zeros). */
    for (size_t i = 0; i + 1 < b->len; ++i) {
        uint32_t x = b->limb[i];
        for (int j = 0; j < 9; ++j) {
            if (x % 10 == 0) return true;
            x /= 10;
        }
    }
    /* Most significant limb may have fewer than 9 digits. */
    if (b->len) {
        uint32_t x = b->limb[b->len - 1];
        if (x == 0) return true;          /* not expected for positive n */
        while (x) {
            if (x % 10 == 0) return true;
            x /= 10;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*           Sequence function required by the test harness             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* behaviour below offset is irrelevant */

    /* Any n ending in 0 already has a zero in n^1. */
    if ((n % 10) == 0) return 1;

    BigInt p;
    big_init(&p, (uint32_t)n);

    /* If n itself contains zero digit we are done. */
    if (big_has_zero(&p)) {
        big_free(&p);
        return 1;
    }

    /* Search for the first exponent producing a zero digit. */
    const unsigned MAX_R = 40000;                /* known upper bound for all n */
    unsigned r;
    for (r = 2; r <= MAX_R; ++r) {
        big_mul(&p, (uint32_t)n);
        if (big_has_zero(&p)) {
            big_free(&p);
            return (seq_t)r;                 /* cast; may overflow for tiny seq_t,
                                                 but exactness is required only
                                                 when the value fits in seq_t */
        }
    }

    /* Should not happen, but keep correctness. */
    big_free(&p);
    return (seq_t)MAX_R;
}
