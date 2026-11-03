/*
 * A054027  Numbers that do not divide their sum of divisors.
 * https://oeis.org/A054027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054027: Numbers that do not divide their sum of divisors. */
/* The term a(n) is the n-th integer m ≥ 2 for which m does NOT divide σ(m),
   where σ(m) is the sum of all positive divisors of m. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Return true iff n divides its sum of divisors σ(n). */
static bool divides_sigma(uint64_t n)
{
    uint64_t sigma = 1;            /* product of (p^{e+1}-1)/(p-1) of each prime power */
    uint64_t temp   = n;

    for (uint64_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            uint64_t power = 1;
            uint64_t term  = 1;
            while (temp % p == 0) {
                temp   /= p;
                power  *= p;
                term   += power;   /* sum of p^0 + p^1 + … + p^e */
            }
            sigma *= term;
        }
    }

    if (temp > 1)      /* remaining prime factor */
        sigma *= (1 + temp);

    return (sigma % n) == 0;
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for n = 0; guard clause */
        return (seq_t)0;

    idx_t   count = 0;           /* how many valid terms found so far */
    for (uint64_t m = 2; ; ++m) {
        if (!divides_sigma(m)) {  /* m ∤ σ(m) → include in sequence */
            ++count;
            if (count == n)
                return (seq_t)m;   /* cast to seq_t for the build's type */
        }
    }
}
