/*
 * A204989  Least 2^k such that n divides 2^k-2^j for some j<k.
 * https://oeis.org/A204989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A204989.
   Let n = 2^e * m with m odd.  For m=1 the order of 2 modulo m is 1.
   The least k such that some j<k with j≥max(1,e) satisfies n | 2^k-2^j is
   k = max(1,e) + ord_m(2).  The sequence value is 2^k. */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Extract 2-adic valuation e of n */
    idx_t e = 0;
    idx_t nn = n;
    while ((nn & 1ULL) == 0ULL) {
        e++;
        nn >>= 1;
    }
    /* Odd part m */
    idx_t m = nn;          /* m is odd (m = n >> e) */

    /* Compute multiplicative order of 2 modulo m */
    idx_t t = 1;
    if (m != 1) {
        idx_t mod = 2 % m;     /* 2 mod m (non-zero here) */
        while (mod != 1) {
            mod = (mod * 2) % m;
            t++;
        }
    }   /* else t remains 1 for m==1 */

    /* Minimum j that can be used: j0 = max(1, e) */
    idx_t j0 = (e >= 1) ? e : 1;

    /* Desired exponent k = j0 + t */
    idx_t k = j0 + t;

    return pow2(k);
}
