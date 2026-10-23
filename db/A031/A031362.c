/*
 * A031362  Number of coincidence site modules of index 10n+1 with 10-fold
 *          symmetry in plane.
 * https://oeis.org/A031362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031362: number of coincidence site modules of index 10n+1 with 10‑fold symmetry.
   For index n (n ≥ 1) the integer whose coefficient is required is
   m = 10*(n-1)+1 (the numbers 1,11,21,31,…).
   The Dirichlet series shows that the arithmetic function is multiplicative:
       a(1) = 1
       a(m) = 0  if m has a prime factor p ≢ 1 (mod 5)
       a(m) = ∏_{p|m, p≡1 (mod 5)} 4·e(p),  where e(p) is the exponent of p in m
   Consequently the function below factors m, checks all prime factors,
   and multiplies 4·e for each admissible prime. */

/* ========================================================================== */
#include <stdint.h>

seq_t a(idx_t n)
/* -------------------------------------------------------------------------- */
{
    /* n should be >= 1 by contract. For safety, return 0 for n == 0. */
    if (n == 0) return 0;

    /* The integer whose coefficient we need. */
    idx_t m = 10 * (n - 1) + 1;

    seq_t result = 1;
    idx_t temp = m;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            if (p % 5 != 1) {
                /* Found a prime factor not ≡ 1 (mod 5); coefficient is zero. */
                return 0;
            }
            result *= (seq_t)(4 * e);
        }
    }

    /* Remaining prime factor > sqrt(m), if any. */
    if (temp > 1) {
        if (temp % 5 != 1) {
            return 0;
        }
        /* exponent of the remaining prime is 1. */
        result *= (seq_t)4;
    }

    return result;
}
