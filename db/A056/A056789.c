/*
 * A056789  a(n) = Sum_{k=1..n} lcm(k,n)/gcd(k,n).
 * https://oeis.org/A056789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 841 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056789
   Formula: a(n) = sum_{k=1..n} lcm(k,n)/gcd(k,n)
   Equivalent: a(n) = 1 + (1/2) * sum_{d|n, d>1} φ(d³)          (OEIS)
   where φ is Euler's totient function.
   Code uses only integer arithmetic and works for any unsigned
   seq_t and idx_t defined by the build. */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for fixed width integer types */
#include "../utils/ipow_idx.h"

/* Multiply two seq_t values using a wider type when available.         */
static inline seq_t mul_seq_u128(seq_t a, seq_t b)
{
    return (seq_t)((unsigned __int128)a * b);
}

/* Recursive enumeration of all divisors of n, accumulating the sum
   of φ(d³)/2 for d > 1.  cur_d and cur_phi are the current partial
   divisor and its φ(d³) value. */
static void rec_sum(idx_t idx, size_t pcnt,
                    const idx_t *primes, const int *exps,
                    seq_t cur_d, seq_t cur_phi,
                    unsigned __int128 *sum)
{
    if (idx == pcnt)
    {
        if (cur_d > 1u) *sum += (unsigned __int128)cur_phi >> 1;
        return;
    }

    idx_t p = primes[idx];
    int   emax = exps[idx];
    seq_t p_pow = 1;        /* p^e */
    for (int e = 0; e <= emax; ++e)
    {
        seq_t new_d   = mul_seq_u128(cur_d, p_pow);
        seq_t new_phi = cur_phi;

        if (e > 0)
        {
            /* φ(p^{3e}) = p^{3e-1} * (p-1)   */
            seq_t phi_comp = mul_seq_u128(ipow_idx(p, 3 * e - 1), p - 1);
            new_phi = mul_seq_u128(cur_phi, phi_comp);
        }

        rec_sum(idx + 1, pcnt, primes, exps, new_d, new_phi, sum);

        /* prepare for next e */
        p_pow = mul_seq_u128(p_pow, p);   /* multiply by p to get p^{e+1} */
    }
}

/* ------------------------------------------------------------------ */
/* Internal helpers                                                      */
/* ------------------------------------------------------------------ */

/* Factorise n into prime‑power arrays.                                 */
static void factorize(idx_t n, idx_t *primes, int *exps, size_t *cnt)
{
    *cnt = 0;
    for (idx_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[(*cnt)++] = p;
            exps[ (*cnt) - 1] = e;
        }
    }
    if (n > 1)        /* remaining prime factor */
    {
        primes[(*cnt)++] = n;
        exps[ (*cnt) - 1] = 1;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* safety – OEIS offset is 1 */
    if (n == 1) return (seq_t)1;/* a(1) = 1 */

    /* Factorise n */
    idx_t primes[64];
    int   exps[64];
    size_t pcnt;
    factorize(n, primes, exps, &pcnt);

    /* Sum over all divisors d > 1 of φ(d³)/2.  accumulates in 128‑bit */
    unsigned __int128 sum128 = 0;
    rec_sum(0, pcnt, primes, exps, (seq_t)1, (seq_t)1, &sum128);

    /* Final result: a(n) = 1 + sum */
    unsigned __int128 result128 = 1 + sum128;
    return (seq_t)result128;
}
