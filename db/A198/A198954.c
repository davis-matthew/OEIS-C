/*
 * A198954  Expansion of the rotational partition function for a heteronuclear
 *          diatomic molecule.
 * https://oeis.org/A198954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 0 if n is not a triangular number; * otherwise a(n) =
 *            sqrt(1 + 8*n) which equals 2*k + 1 where * k(k+1)/2 = n
 * Range:     u8 n <= 8255, u16 n <= 8265, u32 n <= 8265, u64 n <= 8265,
 *            u128 n <= 8265, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t low  = 0;
    /* Upper bound for k: for n ≤ 2⁶⁴‑1 the maximum k < 6.1×10⁹.
       1<<33 is safely above that. */
    idx_t high = (idx_t)1 << 33;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t tri = ( (__uint128_t)mid * (mid + 1) ) / 2;

        if (tri == n) {
            return (seq_t)(2 * mid + 1);
        } else if (tri < n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return 0;
}
