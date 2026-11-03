/*
 * A100867  Smallest positive integer k such that A000037(n) is not a
 *          quadratic residue modulo k.
 * https://oeis.org/A100867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A100867:
   a(n) = smallest k >= 2 such that A000037(n)
   (the n-th positive nonsquare integer) is not a quadratic residue modulo k
   formula: a(n) = A139401(A000037(n)) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

/* quadratic residue test for modulus 2^e, e >= 1 */
static inline bool qr_mod_2_pow(seq_t m, idx_t e)
{
    seq_t mod = ((seq_t)1) << e;                  /* 2^e  (fits in seq_t for e <= 63) */
    seq_t r = m % mod;
    if (e == 1)              /* modulo 2 – all residues are squares */
        return true;
    if (e == 2)              /* modulo 4 – residues 0 or 1 */
        return (r == 0) || (r == 1);
    /* e >= 3 – residues modulo 2^e are those with r % 8 in {0,1,4} */
    seq_t r8 = r & 7;        /* r % 8 */
    return (r8 == 0) || (r8 == 1) || (r8 == 4);
}

/* fast modular exponentiation, works for all seq_t types */
static inline seq_t modpow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1 % mod;
    base %= mod;
    while (exp > 0)
    {
        if (exp & 1)
            result = (base * result) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* quadratic residue test for modulus p^e, p odd prime, e >= 1 */
static inline bool qr_mod_prime_pow(seq_t m, idx_t p, idx_t e)
{
    /* compute p^e */
    idx_t pow = 1;
    for (idx_t i = 0; i < e; ++i)
        pow *= p;

    seq_t mp = m % (seq_t)pow;          /* m modulo p^e */

    if (mp == 0)                     /* divisible by p^e, always a square */
        return true;

    /* determine valuation k = v_p(mp) */
    idx_t k = 0;
    seq_t tmp = mp;
    while (tmp % (seq_t)p == 0)
    {
        ++k;
        tmp /= (seq_t)p;
    }

    /* if k is odd, mp cannot be a square modulo p^e */
    if (k & 1)
        return false;

    /* u = mp / p^k  (now p ∤ u) */
    seq_t u = tmp % (seq_t)p;          /* u mod p */

    /* Legendre symbol: u^((p-1)/2) mod p == 1 */
    seq_t leg = modpow_seq(u, ((seq_t)p - 1) >> 1, (seq_t)p);
    return leg == 1;
}

/* check whether m is a quadratic residue modulo k (k >= 2) */
static inline bool is_quadratic_residue_mod_k(seq_t m, idx_t k)
{
    idx_t rem = k;

    /* factor out powers of 2 */
    if ((rem & 1U) == 0)
    {
        idx_t e = 0;
        while ((rem & 1U) == 0)
        {
            rem >>= 1;
            ++e;
        }
        if (!qr_mod_2_pow(m, e))
            return false;
    }

    /* factor odd primes */
    for (idx_t p = 3; p * p <= rem; p += 2)
    {
        if (rem % p == 0)
        {
            idx_t e = 0;
            while (rem % p == 0)
            {
                rem /= p;
                ++e;
            }
            if (!qr_mod_prime_pow(m, p, e))
                return false;
        }
    }

    /* remaining rem > 1 is an odd prime with exponent 1 */
    if (rem > 1)
    {
        if (!qr_mod_prime_pow(m, rem, 1))
            return false;
    }

    return true;
}

/* return the n-th non‑square integer, i.e., A000037(n) */
static inline seq_t nth_non_square(idx_t n)
{
    idx_t count = 0;
    idx_t m = 0;
    while (count < n)
    {
        ++m;
        /* test for perfect square using isqrt_u64 */
        idx_t r = isqrt(m);
        if (r * r != m)
            ++count;
    }
    return (seq_t)m;
}

/* sequence a(n) */
seq_t a(idx_t n)
{
    seq_t m = nth_non_square(n);       /* A000037(n) */
    for (idx_t k = 2; ; ++k)
    {
        if (!is_quadratic_residue_mod_k(m, k))
            return (seq_t)k;          /* minimal k with non‑residue property */
    }
}
