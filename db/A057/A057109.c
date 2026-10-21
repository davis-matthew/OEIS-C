/*
 * A057109  Numbers n that are not factors of P(n)!, where P(n) is the largest
 *          prime factor of n.
 * https://oeis.org/A057109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 2102, u32 n <= 9556, u64 n <= 9556,
 *            u128 n <= 9556, big any n
 * Helpers:   factorial_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A057109 – Numbers n that are not factors of P(n)!,
    where P(n) is the largest prime factor of n.
    This implementation builds the sequence on‑demand by
    testing successive integers.  Each integer is
    factorised by simple trial division; for every prime
    factor q of n we compute the exponent of q in
    P(n)! via Legendre's formula.  If any exponent in n
    exceeds the corresponding factorial exponent then
    n is appended to the sequence.  The construction
    is linear in the number of integers examined, and
    the algorithm is fast enough for n up to at least
    100 000 on typical modern hardware.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorial_exponent.h"

/* Types supplied by the build system */
extern seq_t a(idx_t n);          /* declaration only; real definition follows */

/* ------------------------------------------------------------------ */
/*  Sequence cache – stores the first terms that have been found   */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache   = NULL;   /* array of sequence values   */
static size_t seq_cap     = 0;      /* allocated capacity         */
static size_t seq_len     = 0;      /* number of entries stored   */
static idx_t  last_checked  = 1;    /* last integer that has
                                      been examined            */

/* Append a new term to the cache, growing the buffer if needed */
static void cache_append(seq_t val)
{
    if (seq_len == seq_cap)
    {
        seq_cap = seq_cap ? seq_cap << 1 : 64;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
    }
    seq_cache[seq_len++] = val;
}

/* --------------------------------------------------------------- */
/*  Factorisation of a single integer by trial division            */
/* --------------------------------------------------------------- */
static void factorise(idx_t n,
                     idx_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    idx_t tmp = n;
    for (idx_t p = 2; (p * p) <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            primes[(*cnt)] = p;
            exps[(*cnt)]  = e;
            ++(*cnt);
        }
    }
    if (tmp > 1)          /* remaining prime factor   */
    {
        primes[*cnt] = tmp;
        exps[*cnt]   = 1;
        ++(*cnt);
    }
}

/* ------------------------------------------------------------------ */
/*  True iff n does NOT divide P(n)!                                 */
/* ------------------------------------------------------------------ */
static bool fails_factorial(idx_t n)
{
    /* Gather all prime factors of n */
    idx_t primes[64];
    int  exps[64];
    size_t cnt;

    factorise(n, primes, exps, &cnt);

    /* Determine the largest prime factor of n */
    idx_t largest = 0;
    for (size_t i = 0; i < cnt; ++i)
        if (primes[i] > largest) largest = primes[i];

    /* Check each prime factor against the factorial exponent of
       the largest prime factor.                                          */
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t eFact = factorial_exponent(largest, primes[i]);
        if ((seq_t)exps[i] > eFact)   /* exponent in n is too large */
            return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/*  Main entry point for the sequence                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for 0          */

    /* Generate terms until we have at least n of them */
    while (seq_len < n)
    {
        ++last_checked;                 /* next integer to test      */
        if (fails_factorial(last_checked))
            cache_append((seq_t)last_checked);
    }
    return seq_cache[n - 1];            /* 0‑based indexing in cache*/
}
