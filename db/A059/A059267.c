/*
 * A059267  Numbers n with 2 divisors d1 and d2 having difference 2: d2 - d1 =
 *          2; equivalently, numbers that are 0 (mod 4) or have a divisor d of
 *          the form d = m^2 - 1.
 * https://oeis.org/A059267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer that is either a multiple of 4
 *            or has a divisor of the form m^2–1 (m≥2)
 * Range:     u8 n <= 132, u16 n <= 3131, u32 n <= 3131, u64 n <= 3131,
 *            u128 n <= 3131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t count = 0;            /* how many qualifying numbers found  */
    for (idx_t k = 1; ; ++k)    /* candidate integers, 1‑based            */
    {
        seq_t cand = (seq_t)k;

        /* Multiples of 4 automatically qualify.  */
        if (cand % (seq_t)4 == (seq_t)0)
        {
            ++count;
            if (count == n) return cand;
            continue;
        }

        /* Otherwise check all divisors of the form m^2–1 ≤ cand.  */
        for (uint64_t m = 2; ; ++m)
        {
            seq_t d = (seq_t)m * (seq_t)m - (seq_t)1;
            if (d > cand) break;
            if (cand % d == (seq_t)0)
            {
                ++count;
                if (count == n) return cand;
                break;          /* found a qualifying divisor */
            }
        }
    }
}
