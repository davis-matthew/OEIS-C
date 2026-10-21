/*
 * A083117  Smallest k such that k*n contains a single digit with
 *          multiplicity, or 0 if no such number exists.
 * https://oeis.org/A083117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive integer k such that k*n is a repdigit *
 *            (all decimal digits equal). If no such k exists, a(n) = 0. * The
 *            sequence has a(n)=0 precisely when n is divisible by 10, 16 or
 *            25
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 16, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   idx_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/idx_gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n == 0) return (seq_t)0;

    /* Quick elimination: no repdigit multiple exists when n has 2⁴, 5² or 2·5. */
    if ((n % 10) == 0 || (n % 16) == 0 || (n % 25) == 0)
        return (seq_t)0;

    bool found = false;
    seq_t best = 0;

    for (idx_t d = 1; d <= 9; ++d) {
        /* Reduce the problem to finding a repunit R_m divisible by n' */
        idx_t g = idx_gcd(n, d);
        idx_t nprime = n / g;          /* n' = n / gcd(n,d) */
        if (nprime % 2 == 0 || nprime % 5 == 0)
            continue;                  /* no repunit can be divisible by n' */

        idx_t r_mod = 1 % nprime;     /* R_1 mod n'  */
        seq_t R = 1;                  /* exact value of R_m */

        /* Iterate over repunits R_m until n' | R_m or we have tried all residues. */
        for (idx_t m = 1; ; ++m) {
            if (r_mod == 0) {
                seq_t dprime = d / g;   /* d' = d / gcd(n,d)  */
                seq_t cand = dprime * (R / (seq_t)nprime);
                if (!found || cand < best) {
                    best = cand;
                    found = true;
                }
                break;                  /* minimal m found for this digit d */
            }
            if (m >= nprime) {          /* no solution for this digit */
                break;
            }
            r_mod = (r_mod * 10 + 1) % nprime;   /* advance: R_{m+1} = R_m*10 + 1  */
            R = R * 10 + 1;
        }
    }

    return found ? best : (seq_t)0;
}
