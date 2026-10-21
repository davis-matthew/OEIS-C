/*
 * A054850  Binary logarithm of n-th primorial, rounded down to an integer.
 * https://oeis.org/A054850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(log₂ ∏_{i=1}^{n} p_i) (primorial of the n‑th prime)
 * Range:     u8 n <= 43, u16 n <= 4733, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/sieve_first_n_primes.h"

/* Simple big integer type that can hold numbers up to a few megabytes. */
typedef struct {
    uint64_t *limb;   /* least‑significant limb first */
    size_t size;      /* number of used limbs */
    size_t capacity;  /* allocated limbs */
} bigint_t;

/* initialise a big integer to value 1 */
static void bigint_init(bigint_t *b)
{
    b->capacity = 4;
    b->size = 1;
    b->limb = (uint64_t *)malloc(b->capacity * sizeof(uint64_t));
    b->limb[0] = 1ULL;
}

/* free the resources used by a big integer */
static void bigint_free(bigint_t *b)
{
    free(b->limb);
}

/* multiply the big integer by a 32‑bit unsigned value */
static void bigint_mul_uint32(bigint_t *b, uint32_t m)
{
    unsigned __int128 carry = 0;
    for (size_t i = 0; i < b->size; ++i) {
        unsigned __int128 prod = (unsigned __int128)b->limb[i] * m + carry;
        b->limb[i] = (uint64_t)prod;
        carry = prod >> 64;
    }
    while (carry) {
        if (b->size == b->capacity) {
            b->capacity <<= 1;
            b->limb = (uint64_t *)realloc(b->limb, b->capacity * sizeof(uint64_t));
        }
        b->limb[b->size++] = (uint64_t)carry;
        carry >>= 64;
    }
}

/* return the bit length (position of highest set bit + 1) of the big integer */
static size_t bigint_bitlen(const bigint_t *b)
{
    uint64_t msw = b->limb[b->size - 1];
    int clz = __builtin_clzll(msw);          /* count leading zeros */
    return (b->size - 1) * 64 + (64 - clz);
}

/* --- main sequence function --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset 1 in OEIS, but keep defined for safety */

    /* Get the first n primes (first n primalloor values). */
    uint32_t *primes = NULL;
    size_t primes_cnt = 0;
    sieve_first_n_primes(n, &primes, &primes_cnt);

    /* Compute the primorial as a big integer and find its bit length. */
    bigint_t prod;
    bigint_init(&prod);
    for (size_t i = 0; i < (size_t)n; ++i) {
        bigint_mul_uint32(&prod, primes[i]);
    }

    size_t bits = bigint_bitlen(&prod);   /* floor(log₂(product)) + 1 */
    bigint_free(&prod);
    free(primes);

    return (seq_t)(bits - 1);             /* floor(log₂(product)) */
}
