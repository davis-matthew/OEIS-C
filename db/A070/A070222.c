/*
 * A070222  Numbers k such that the sum of prime divisors of k divides the sum
 *          of divisors of k.
 * https://oeis.org/A070222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 486, u32 n <= 4600, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 8456 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the sum of prime divisors of k divides sigma(k)
   (OEIS A070222).
   Implementation: trial‑division factorisation, computation of
   σ(k) = Π (p^(e+1)-1)/(p-1) and of the sum of distinct prime divisors.
   Results are cached in a static array so that calls to a(n) can be made
   in arbitrary order. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Factorise n into distinct prime factors and their exponents.
   fac[0..cnt-1]   : distinct prime factors (each < 2^63)
   exp[0..cnt-1]   : corresponding exponents
   cnt             : number of distinct factors (≤ 64)            */
static void factor_uint64_own(uint64_t n, uint64_t fac[64], uint8_t exp[64], size_t *cnt)
{
    *cnt = 0;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            fac[(*cnt)] = p;
            uint8_t e = 0;
            do { x /= p; ++e; } while (x % p == 0);
            exp[(*cnt)++] = e;
        }
    }
    if (x > 1) {
        fac[(*cnt)] = x;
        exp[(*cnt)++] = 1;
    }
}

/*=== Memoisation storage ===*/
static seq_t *terms      = NULL;    /* stored sequence values           */
static size_t terms_len  = 0;       /* number of values stored          */
static size_t terms_cap  = 0;       /* current array capacity           */
static idx_t  last_k     = 0;       /* largest integer already tested   */

/* Append a value to the memoised list, expanding the array if necessary. */
static void add_term(seq_t v)
{
    if (terms_len == terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
        if (!tmp) abort();                     /* out of memory */
        terms  = tmp;
        terms_cap = new_cap;
    }
    terms[terms_len++] = v;
}

/*----------------------------------------------------------*/
/*  Public interface: returns the n-th term   (n>=1)        */
/*----------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not defined for n=0 */

    /* generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        ++last_k;                         /* test next integer */

        uint64_t facs[64];
        uint8_t exps[64];
        size_t  nf;

        factor_uint64_own(last_k, facs, exps, &nf);

        seq_t sum_primes = 0;             /* sum of distinct primes */
        seq_t sigma     = 1;              /* multiplicative sigma  */

        for (size_t i = 0; i < nf; ++i) {
            sum_primes += (seq_t)facs[i];

            /* compute term_i = (p^(e+1)-1)/(p-1) */
            seq_t term = 1;
            seq_t p    = (seq_t)facs[i];
            /* p^(e+1) */
            for (uint8_t j = 0; j < exps[i] + 1; ++j)
                term *= p;
            term -= 1;                  /* p^(e+1)-1 */
            term /= (p - 1);            /* divide by p-1   */
            sigma *= term;
        }

        if (sum_primes != 0 && sigma % sum_primes == 0)
            add_term((seq_t)last_k);
    }

    return terms[n - 1];
}
