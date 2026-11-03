/*
 * A076685  a(n) = max(core(n),phi(n)) where core(n) is the squarefree part of
 *          n.
 * https://oeis.org/A076685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max(core(n), phi(n)) where core(n) is the squarefree part
 *            of n, defined as n divided by the largest square dividing n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9899 (2026-11-01)
 * Helpers:   totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the provided phi routine and an
   inline factorisation to compute the squarefree part. */

#include <stdint.h>
#include "../utils/totient.h"

/* Compute the squarefree part of n: product of primes with odd exponent */
static seq_t core_squarefree_part(idx_t n)
{
    uint64_t N = (uint64_t)n;         /* n fits in 64 bits (as idx_t)        */
    seq_t core = (seq_t)1;

    /* Handle factor 2 separately */
    if (N & 1ULL) {
        /* nothing to do: 2 is odd, include it directly below */
    }
    int odd; /* 0 or 1 for whether the current prime had odd exponent */

    /* factor 2 */
    if (N % 2ULL == 0) {
        odd = 0;
        while (N % 2ULL == 0ULL) {
            N /= 2ULL;
            odd ^= 1;          /* toggle parity on each factor of 2 */
        }
        if (odd)
            core *= (seq_t)2;
    }

    /* factor odd primes */
    for (uint64_t p = 3ULL; p <= N / p; p += 2ULL) {
        if (N % p == 0ULL) {
            odd = 0;
            while (N % p == 0ULL) {
                N /= p;
                odd ^= 1;     /* toggle parity on each factor of p */
            }
            if (odd)
                core *= (seq_t)p;
        }
    }

    /* any remaining prime factor with exponent 1 */
    if (N > 1ULL)
        core *= (seq_t)N;

    return core;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but return 0 to avoid UB */
        return (seq_t)0;

    seq_t core = core_squarefree_part(n);
    seq_t phi_val = totient((uint64_t)n);

    return (core > phi_val) ? core : phi_val;
}
