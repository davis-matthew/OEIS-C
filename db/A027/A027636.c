/*
 * A027636  Expansion of (1+x^19)/((1-x^2)*(1-x^4)^2*(1-x^6)).
 * https://oeis.org/A027636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 327, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 946 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coeff_0(n) + coeff_0(n-19)
 * where coeff_0(k) counts the number of solutions of  
 *   2x + 4y + 4z + 6w = k,  with x,y,z,w >= 0.  
 * An explicit closed form for coeff_0(k) is
 *   coeff_0(k) = Σ_{d=0}^{⌊k/3⌋} ((⌊(k-3d)/2⌋ + 1)·(⌊(k-3d)/2⌋ + 2)) / 2 .
 * This avoids any subtraction on unsigned types and uses only additions
 * and multiplications that fit into seq_t.  The function is valid for
 * all n ≥ 0 and follows the OEIS definition of A027636.
 */

#include <stdbool.h>

/* Helper: compute coeff_0(k) as described above */
static seq_t coeff0(idx_t k)
{
    /* no solutions for odd k */
    if (k & 1U) return 0;

    seq_t total = 0;
    idx_t m = k / 2U;                 /* because all terms are even */
    idx_t max_d = m / 3U;            /* d ranges 0 .. floor(m/3) */

    for (idx_t d = 0U; d <= max_d; ++d)
    {
        /* remaining after taking 3d from m */
        idx_t rem = m - 3U * d;
        idx_t t   = rem / 2U;        /* floor((m-3d)/2) */
        /* (t+1)(t+2)/2 fits into seq_t if seq_t is big enough */
        seq_t val = ((seq_t)(t) + 1U) * ((seq_t)(t) + 2U) / 2U;
        total += val;
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t res = coeff0(n);
    if (n >= 19U)
        res += coeff0(n - 19U);
    return res;
}
