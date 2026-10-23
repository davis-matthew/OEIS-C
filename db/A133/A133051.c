/*
 * A133051  Squares of perfect numbers.
 * https://oeis.org/A133051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000396(n)^2 (square of the nth perfect number)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>          /* for size_t and NULL */
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* provides seq_pow(seq_t, idx_t) */

static const uint64_t mersenne_exponents[] = {
    /* List of exponents p such that 2^p‑1 is prime (known Mersenne primes). */
    2ULL, 3ULL, 5ULL, 7ULL, 13ULL, 17ULL, 19ULL, 31ULL, 61ULL, 89ULL,
    107ULL, 127ULL, 521ULL, 607ULL, 1279ULL, 2203ULL, 2281ULL, 3217ULL,
    4253ULL, 4423ULL, 9689ULL, 9941ULL, 11213ULL, 19937ULL, 21701ULL,
    23209ULL, 44497ULL, 86243ULL, 110443ULL, 132049ULL, 216091ULL,
    756839ULL, 859433ULL, 1257787ULL, 1398269ULL, 2976221ULL,
    3021377ULL, 3022729ULL, 3258267ULL, 3317045ULL, 3371419ULL,
    3581509ULL, 3995941ULL, 4354669ULL, 4817597ULL, 5325383ULL,
    6209189ULL, 6531199ULL, 9663721ULL, 10378673ULL, 10934689ULL
};

static const size_t NUM_EXPS = sizeof(mersenne_exponents) / sizeof(mersenne_exponents[0]);

/* External linkage, per requirement. */
seq_t a(idx_t n)
{
    if (n < 1)                /* OEIS offset is 1; return 0 for n < 1. */
        return (seq_t)0;

    size_t idx = (size_t)n - 1;      /* convert to 0‑based index */
    if (idx >= NUM_EXPS)             /* unknown beyond the known list */
        return (seq_t)0;

    uint64_t p = mersenne_exponents[idx];
    /* 2^p  */
    seq_t pow2p = seq_pow((seq_t)2, (idx_t)p);
    /* 2^p – 1  */
    seq_t mersenne = pow2p - ((seq_t)1);
    /* 2^(p‑1)  */
    seq_t pow2p1 = seq_pow((seq_t)2, (idx_t)(p - 1));

    /* Perfect number: 2^(p‑1) * (2^p – 1)  */
    seq_t perfect = pow2p1 * mersenne;

    /* Square the perfect number */
    return perfect * perfect;
}
