/*
 * A096896  Least hypotenuse of primitive Pythagorean triangles with even leg
 *          4n.
 * https://oeis.org/A096896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A096896: least hypotenuse of a primitive Pythagorean triangle
   in which the even leg equals 4 n.
   For given n>=1, find all factor pairs (d,e) with d*e = 2n,
   gcd(d,e)=1 and d,e of opposite parity.  The corresponding
   primitive triple has hypotenuse z = d²+e².  The minimum of all such z
   is the sequence value a(n).
*/
#include <stdint.h>
#include "../utils/gcd64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* behavior below offset is irrelevant */

    uint64_t two_n = n * 2ull;           /* product 2*n, fits in 64‑bit for all used n */

    unsigned __int128 best = (unsigned __int128)-1;   /* huge initial value */

    /* iterate over divisors d of 2*n up to sqrt(2*n) */
    for (uint64_t d = 1; d * d <= two_n; ++d) {
        if (two_n % d != 0) continue;   /* d must divide 2*n */

        uint64_t e = two_n / d;
        if (d == e) continue;           /* m>k required */

        /* parity condition: one even, one odd */
        if ((d & 1) == (e & 1)) continue;
        /* coprime condition for primitiveness */
        if (gcd64(d, e) != 1) continue;

        /* compute candidate hypotenuse as 128‑bit to avoid overflow */
        unsigned __int128 cand = (unsigned __int128)d * d
                               + (unsigned __int128)e * e;
        if (cand < best) best = cand;
    }

    return (seq_t)best;
}
