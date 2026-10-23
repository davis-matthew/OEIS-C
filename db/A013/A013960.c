/*
 * A013960  a(n) = sigma_12(n), the sum of the 12th powers of the divisors of
 *          n.
 * https://oeis.org/A013960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = sigma_12(n) = Sum_{d|n} d^12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 40, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses a fast smallest‑prime‑factor sieve to compute the
   multiplicative divisor‑sum function σ₁₂(n). The implementation
   works for any idx_t >= 1 and for arbitrary‑precision seq_t types. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t *spf = NULL;        /* smallest prime factor table */
static idx_t    spf_size = 0;       /* current size of spf table */

/* Build or extend the smallest‑prime‑factor table up to at least n.  */
static void build_spf(idx_t n)
{
    if (n <= spf_size)
        return;

    uint64_t *new_spf = (uint64_t *)realloc(spf, sizeof(uint64_t) * (n + 1));
    if (!new_spf)
        exit(1);
    spf = new_spf;
    for (idx_t i = spf_size + 1; i <= n; ++i)
        spf[i] = i;                 /* initialise with itself */

    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i) {
        if (spf[i] == i) {                   /* i is prime */
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j)
                    spf[j] = i;             /* set smallest prime factor */
        }
    }
    spf_size = n;
}

/* Compute σ₁₂(n) = sum of the 12th powers of the divisors of n.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* defined only for n ≥ 1 */

    build_spf(n);                  /* ensure spf table covers n */
    uint64_t tmp = (uint64_t)n;    /* work in 64‑bit for division */

    seq_t result = 1;              /* multiplicative accumulation */

    while (tmp > 1) {
        uint64_t prime = spf[tmp];
        size_t   exp   = 0;

        /* factor out the current prime */
        while (tmp % prime == 0) {
            tmp    /= prime;
            exp++;
        }

        /* compute σ₁₂(p^exp) = sum_{k=0..exp} p^{12k} */
        seq_t term = 0;
        seq_t pow  = 1;          /* p^{12k} */
        seq_t p_power12 = 1;     /* p^12 */

        for (int i = 0; i < 12; ++i)
            p_power12 *= (seq_t)prime;

        for (size_t k = 0; k <= exp; ++k) {
            term += pow;        /* add current term */
            pow  *= p_power12;  /* advance to next power */
        }

        result *= term;         /* multiply into overall product */
    }

    return result;
}
