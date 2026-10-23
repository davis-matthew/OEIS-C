/*
 * A027636  Expansion of (1+x^19)/((1-x^2)*(1-x^4)^2*(1-x^6)).
 * https://oeis.org/A027636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 327, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   coeff0
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
#include "../utils/coeff0.h"

seq_t a(idx_t n)
{
    seq_t res = coeff0(n);
    if (n >= 19U)
        res += coeff0(n - 19U);
    return res;
}
