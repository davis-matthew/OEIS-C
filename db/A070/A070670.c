/*
 * A070670  Smallest m in range 2..n-1 such that m^5 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m in [2, n-1] such that m^5 ≡ 1 (mod n), or 1 if
 *            no such m exists
 * Range:     u8 n <= 268, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod_seq, pow5_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_mod_seq.h"
#include "../utils/pow5_mod.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;          /* a(1) = a(2) = 1 by definition */

    seq_t mod = (seq_t)n;
    idx_t m;

    /* Search in the range 2 … n-1 for the smallest m such that m^5 ≡ 1 (mod n) */
    for (m = 2; m < n; ++m)
    {
        if (pow5_mod((seq_t)m, mod) == 1)
            return (seq_t)m;      /* return the first (smallest) match */
    }
    return 1;                      /* no such m exists */
}
