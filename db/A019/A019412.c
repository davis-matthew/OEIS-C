/*
 * A019412  Primes with primitive root 90.
 * https://oeis.org/A019412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 90 is a primitive root modulo p
   (OEIS A019412).  A simple brute‑force search is used. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/mod_pow.h"

/* Check whether 90 is a primitive root modulo the prime p. */
static bool is_primitive_root_90(uint64_t p)
{
    /* 90 must be coprime to p – the only prime divisors of 90 are 2,3,5. */
    if (p == 2 || p == 3 || p == 5)
        return false;

    uint64_t phi = p - 1;           /* φ(p) = p-1 for prime p */
    uint64_t prim[64];
    uint64_t exp[64];
    size_t   cnt;

    factor_u64(phi, prim, exp, &cnt);   /* factor φ(p) */

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t q      = prim[i];
        uint64_t exponent = phi / q;      /* (p-1)/q  */
        seq_t    r      = mod_pow((seq_t)90, exponent, (seq_t)p);
        if (r == (seq_t)1)                /* order divides (p-1)/q → not primitive */
            return false;
    }
    return true;
}

/* Memoisation of found primes. */
static seq_t *primes   = NULL;   /* dynamic array of primes with 90 as primitive root */
static size_t size_cap = 0;      /* current allocated capacity */
static idx_t  count    = 0;      /* number of primes stored */
static uint64_t next   = 2;      /* next integer to test */

/* Append a prime to the memoised array, resizing if necessary. */
static void append_prime(uint64_t p)
{
    if (count == size_cap)
    {
        size_cap = size_cap ? size_cap * 2 : 8;
        primes = (seq_t *)realloc(primes, size_cap * sizeof(seq_t));
    }
    primes[count++] = (seq_t)p;
}

/* Make sure that at least n primes with 90 as primitive root have been found. */
static void ensure_n(idx_t n)
{
    while (count < n)
    {
        /* Scan forward for the next prime that satisfies the condition. */
        do
        {
            if (is_prime(next) && is_primitive_root_90(next))
                append_prime(next);
            ++next;
        } while (count < n && next != 0);   /* next != 0 protects against wrap‑around */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset is 1; return 0 for undefined indices */
    ensure_n(n);
    return primes[n - 1];
}
