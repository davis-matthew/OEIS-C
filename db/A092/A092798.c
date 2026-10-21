/*
 * A092798  Numerator of partial products in an approximation of Pi/2.
 * https://oeis.org/A092798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = ∏_{k=1}^{n+1} A122214(k)^{2^{\,n-k+1}} Each a(n) is a
 *            power of two: a(n) = 2^{E(n)} where E(1)=1 and for n≥2 E(n) =
 *            2·E(n-1) + e_{n+1} with e_k = 2^{\,k-2} + (k-3)(k-2)(k-1)/6
 *            (k≥3). The implementation uses this recurrence to compute the
 *            exponent and returns 2^{E(n)} via the helper pow_uin...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

/* Computes a(n) for n ≥ 1 */
seq_t a(idx_t n)
{
    /* Offset: 1 – a(1) is the first element. */
    if (n == 0) return 1;                /* undefined, but safe return */

    /* base case */
    if (n == 1) return pow_uint(2, 1);   /* a(1) = 2 */

    /* exponent of 2 in a(n) */
    idx_t E = 1;                         /* E(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        /* compute e_{i+1} */
        idx_t k   = i + 1;               /* current index in A122214   */
        idx_t part_pow = 0;
        /* 2^{k-2} : k≤64 for the supported n range, so safe shift */
        if (k >= 2)
            part_pow = 1ULL << (k - 2);

        idx_t part_tri = (k - 3) * (k - 2) * (k - 1) / 6;
        idx_t e_k = part_pow + part_tri;

        E = (E << 1) + e_k;
    }

    return pow_uint(2, E);
}
