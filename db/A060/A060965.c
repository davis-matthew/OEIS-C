/*
 * A060965  For p=prime(n), a(n) = number of points (x,y) on the elliptic
 *          curve x^3 + y^3 = 1 (mod p), including the point at infinity.
 * https://oeis.org/A060965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes OEIS sequence A060965.
   a(n) = number of points (x,y) on the elliptic curve
   x^3 + y^3 = 1 over GF(p_n), where p_n is the n-th prime
   (p_0 = 2), including the point at infinity.  */

/* Use the helper that builds all primes up to a given limit. */
#include "../utils/build_primes_upto.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Maximum prime we will need for n up to about 10 000. */
#define MAX_PRIME 200000      /* > prime(10 000) = 104 729 */

static uint64_t *primes = NULL;          /* array of primes */
static size_t primes_cnt = 0;            /* number of primes */
static uint32_t *cubic_stamp = NULL;     /*  stamp array for cubic residues */
static uint32_t stamp_value = 1;         /* current stamp */
static seq_t *cache = NULL;              /* cached sequence values */

/* Initialise prime list and auxiliary arrays.  */
static void init(void)
{
    if (primes != NULL) return;          /* already initialised */

    build_primes_upto((idx_t)MAX_PRIME, &primes, &primes_cnt);

    /* Allocate stamp array big enough for MAX_PRIME entries. */
    cubic_stamp = (uint32_t *)malloc((size_t)MAX_PRIME * sizeof(uint32_t));
    if (cubic_stamp == NULL) {           /* allocation failure – fatal */
        exit(EXIT_FAILURE);
    }

    /* Allocate cache for all primes.  Zeroes mean “not computed yet”. */
    cache = (seq_t *)calloc(primes_cnt, sizeof(seq_t));
    if (cache == NULL) {
        exit(EXIT_FAILURE);
    }
}

/* Return the n-th term of A060965.  */
seq_t a(idx_t n)
{
    init();                 /* ensure everything is ready */

    if (n >= (idx_t)primes_cnt) return (seq_t)0;  /* out of range */

    seq_t cached = cache[(size_t)n];
    if (cached != 0) return cached;              /* already computed */

    uint64_t p = primes[(size_t)n];

    /* For p = 3 or p ≡ 2 (mod 3), the curve is supersingular
       and has exactly p+1 points. */
    if (p == 3 || (p % 3) == 2) {
        cache[(size_t)n] = (seq_t)(p + 1);
        return cache[(size_t)n];
    }

    /* p ≡ 1 (mod 3): count points by marking cubic residues. */
    uint32_t current_stamp = stamp_value++;
    if (current_stamp == 0) {        /* avoid zero stamp */
        current_stamp = stamp_value++;
    }

    /* Mark all cubic residues modulo p. */
    for (idx_t y = 0; y < p; ++y) {
        uint64_t y2 = (y * y) % p;
        uint64_t v   = (y2 * y) % p;    /* y^3 mod p */
        cubic_stamp[(size_t)v] = current_stamp;
    }

    /* Count affine solutions. */
    uint64_t affine = 0;
    for (idx_t x = 0; x < p; ++x) {
        uint64_t x2 = (x * x) % p;
        uint64_t x3 = (x2 * x) % p;      /* x^3 mod p */
        uint64_t c  = (p + 1 - x3) % p;  /* 1 - x^3 (mod p) */

        if (c == 0) {
            affine += 1;                /* only y = 0 satisfies y^3 = 0 */
        } else if (cubic_stamp[(size_t)c] == current_stamp) {
            affine += 3;                /* three cubic roots exist */
        }
    }

    /* Add the point at infinity. */
    seq_t points = (seq_t)(affine + 1);
    cache[(size_t)n] = points;
    return points;
}
