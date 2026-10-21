/*
 * A079584  Number of ones in the binary expansion of n!.
 * https://oeis.org/A079584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1-bits in the binary expansion of n!
 * Range:     u8 n <= 114, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = popcount(n!)

   Implementation details:
   • Compute n! using a custom big‑integer type built from 32‑bit words.
   • Multiply by each integer from 2 to n.
   • Count one‑bits in all words to obtain the answer.
   • Works for all supported definitions of seq_t and idx_t.
   • No external helpers are required.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Simple dynamic big‑integer built from 32‑bit words (little‑endian). */
typedef struct {
    uint32_t *d;   /* words array */
    size_t   len;  /* number of used words */
    size_t   cap;  /* allocated capacity */
} biguint_t;

/* Allocate or enlarge the array so that it can hold at least “n” words. */
static void big_uint_ensure(biguint_t *b, size_t n)
{
    if (b->cap < n) {
        size_t newcap = b->cap ? b->cap * 2 : 16;
        while (newcap < n) newcap <<= 1;
        b->d = (uint32_t *)realloc(b->d, newcap * sizeof(uint32_t));
        b->cap = newcap;
    }
}

/* Initialise the big integer to 1. */
static void big_uint_init_one(biguint_t *b)
{
    big_uint_ensure(b, 1);
    b->d[0] = 1;
    b->len = 1;
}

/* Multiply the big integer by a small integer x (x ≤ UINT32_MAX). */
static void big_uint_mul_small(biguint_t *b, uint32_t x)
{
    if (x <= 1) return;
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t prod = (uint64_t)b->d[i] * x + carry;
        b->d[i]   = (uint32_t)prod;
        carry     = prod >> 32;
    }
    while (carry) {
        if (b->len == b->cap) big_uint_ensure(b, b->len + 1);
        b->d[b->len++] = (uint32_t)carry;
        carry >>= 32;
    }
}

/* Count the number of one-bits in the big integer. */
static uint64_t big_uint_popcount(const biguint_t *b)
{
    uint64_t count = 0;
    for (size_t i = 0; i < b->len; ++i)
        count += __builtin_popcount(b->d[i]);
    return count;
}

/* Free the dynamic memory used by the big integer. */
static void big_uint_free(biguint_t *b)
{
    free(b->d);
    b->d = NULL;
    b->len = b->cap = 0;
}

/* Return the OEIS sequence A079584. */
seq_t a(idx_t n)
{
    /* n = 0 or 1 → 0! = 1 and 1! = 1, which contains one '1' in binary. */
    if (n <= 1) return (seq_t)1;

    biguint_t fact;
    memset(&fact, 0, sizeof(fact));
    big_uint_init_one(&fact);

    /* Compute n! by successive multiplication. */
    for (uint32_t i = 2; i <= (uint32_t)n; ++i)
        big_uint_mul_small(&fact, i);

    uint64_t ones = big_uint_popcount(&fact);
    big_uint_free(&fact);
    return (seq_t)ones;
}
