/*
 * A065959  a(n) = n^3*Product_{distinct primes p dividing n} (1+1/p^3).
 * https://oeis.org/A065959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^3*Product_{distinct primes p dividing n} (1+1/p^3)
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 1547, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Equivalently a(p^e) = p^(3e) + p^(3e-3); a(1) = 1.  */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint_seq.h"

/* Simple trial‑division factorisation of a 64‑bit integer.
   Stores primes and their exponents in the provided arrays.
   The caller must provide arrays large enough for all prime factors. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            pr[c] = p;
            ex[c] = e;
            ++c;
        }
    }
    if (temp > 1) {
        pr[c] = temp;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Computes a(n) for the OEIS sequence A065959. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;          /* Base case */

    /* Factor n into primes.  n fits in uint64, so a table of size 64
       is more than enough for the number of distinct prime factors. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        idx_t   e3  = 3 * (idx_t)e;           /* 3e */
        idx_t   e3m = e3 - 3;                 /* 3e-3  (e>=1 always) */

        seq_t pow1 = pow_uint_seq((seq_t)p, e3);
        seq_t pow2 = pow_uint_seq((seq_t)p, e3m);
        seq_t term = pow1 + pow2;             /* p^(3e) + p^(3e-3) */
        result *= term;
    }
    return result;
}
