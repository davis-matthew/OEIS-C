/*
 * A227944  Number of iterations of "take odd part of phi" (A053575) to reach
 *          1 from n.
 * https://oeis.org/A227944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0 for n > 1: a(n) = a(odd_part(phi(n))) + 1 where phi is
 *            Euler's totient and odd_part removes all 2‑factors
 * Range:     u8 n <= 937, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t count = 0;
    seq_t cur   = (seq_t)n;

    while (cur > (seq_t)1)
    {
        seq_t phi_val = calc_phi(cur);
        cur = odd_part(phi_val);
        ++count;
    }

    return count;
}
