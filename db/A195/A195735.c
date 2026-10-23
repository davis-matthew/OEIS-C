/*
 * A195735  a(n) = 2*sigma(n^2) - sigma(n)^2.
 * https://oeis.org/A195735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*sigma(n^2) - sigma(n)^2 where sigma(m) is the sum of
 *            divisors of m
 * Range:     i8 n <= 6, i16 n <= 36, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   seq_pow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------- includes ----------------------- */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow_idx.h"

/* ---------------------- main function ------------------- */
seq_t a(idx_t n)
{
    if (n < 1)   /* behaviour for indices < 1 is unspecified */
        return 0;

    /* Factorise n: trial division.  For n up to ~10^4 the number of
       distinct prime factors is < 20, so a small fixed-size array is
       sufficient. */
    const size_t MAX_FACTORS = 20;
    idx_t prime[MAX_FACTORS];
    idx_t exp[MAX_FACTORS];
    size_t pf = 0;

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            if (pf == MAX_FACTORS) return 0;  /* safety guard */
            prime[pf] = p;
            exp[pf]   = e;
            ++pf;
        }
    }
    /* Remaining prime factor, if any */
    if (temp > 1)
    {
        if (pf == MAX_FACTORS) return 0;
        prime[pf] = temp;
        exp[pf]   = 1;
        ++pf;
    }

    /* Compute sigma(n) and sigma(n^2) multiplicatively. */
    seq_t sigma_n  = 1;
    seq_t sigma_n2 = 1;
    for (size_t i = 0; i < pf; ++i)
    {
        seq_t p = (seq_t)prime[i];
        idx_t e = exp[i];

        /* σ(p^e)   = (p^(e+1)-1)/(p-1) */
        seq_t term_n  = (seq_pow_idx(p, e + 1) - 1) / (p - 1);
        sigma_n      *= term_n;

        /* σ(p^(2e)) = (p^(2e+1)-1)/(p-1) */
        seq_t term_n2 = (seq_pow_idx(p, 2 * e + 1) - 1) / (p - 1);
        sigma_n2     *= term_n2;
    }

    /* Final formula */
    return 2 * sigma_n2 - sigma_n * sigma_n;
}
