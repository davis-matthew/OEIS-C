/*
 * A219670  Number of n-step paths on cubic lattice from (0,0,0) to (1,0,0)
 *          with moves in any direction on {-1,0,1}^3 and zero moves allowed.
 * https://oeis.org/A219670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C1(n) * C0(n)^2 where C0(n) = coefficient of t^n in (1 +
 *            t + t^2)^n = sum_{c=0..⌊n/2⌋} n! / (c! * c! * (n-2c)!) C1(n) =
 *            coefficient of t^{n+1} in (1 + t + t^2)^n = sum_{k=0..⌊(n-1)/2⌋}
 *            n! / (k! * (k+1)! * (n-2k-1)!)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   c1_term, central_trinomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/c1_term.h"
#include "../utils/central_trinomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t c0 = central_trinomial(n);
    seq_t c1 = c1_term(n);
    seq_t res = c1 * c0 * c0;
    return res;
}
