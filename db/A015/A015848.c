/*
 * A015848  Numbers k such that phi(k + 7) | sigma(k) for k not congruent to 0
 *          (mod 3).
 * https://oeis.org/A015848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 24, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A015848: numbers k such that φ(k + 7) | σ(k) for k ≠ 0 (mod 3).
   The function iterates over k, tests the congruence, and returns the n‑th
   admissible value.  Factorisation is performed by simple trial division.
   The implementation works for any seq_t/idx_t pair supplied by the build
   and uses only the allowed arithmetic and comparison operators. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- Helper: binary exponentiation on seq_t ---------------------------------- */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- σ(n) on uint64_t ------------------------------------------------------- */
static seq_t sigma_uint64(uint64_t n)
{
    seq_t sigma = 1;
    uint64_t tmp  = n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            seq_t term = (pow_uint((seq_t)p, e + 1) - 1) / ((seq_t)p - 1);
            sigma *= term;
        }
    }
    if (tmp > 1)            /* remaining prime factor */
    {
        seq_t term = (pow_uint((seq_t)tmp, 2) - 1) / ((seq_t)tmp - 1);
        sigma *= term;
    }
    return sigma;
}

/* ---------- φ(n) on uint64_t ------------------------------------------------------- */
static seq_t phi_uint64(uint64_t n)
{
    seq_t phi = (seq_t)n;
    uint64_t tmp  = n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            while (tmp % p == 0) tmp /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (tmp > 1)            /* remaining prime factor */
    {
        phi = phi / tmp * (tmp - 1);
    }
    return phi;
}

/* ---------- Main sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based; 0 is undefined */

    idx_t found = 0;     /* number of terms found so far */
    uint64_t k = 1;      /* current candidate k */

    for (;; ++k)
    {
        if (k % 3 == 0) continue;           /* skip multiples of 3 */

        seq_t sigma_k  = sigma_uint64(k);
        seq_t phi_k7   = phi_uint64(k + 7);

        if (phi_k7 != 0 && sigma_k % phi_k7 == 0)
        {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
