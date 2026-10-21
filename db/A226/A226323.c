/*
 * A226323  Number of ordered pairs (i,j) with |i| * |j| <= n and gcd(i,j) <=
 *          1.
 * https://oeis.org/A226323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*A064608(n)+5 for n>0, a(0)=1 * where A064608(n) is the
 *            number of ordered coprime pairs * (i,j) with 1 ≤ i,j and i*j ≤
 *            n. The +5 accounts for the * 5 pairs involving at least one zero
 *            coordinate
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t */
#include "../utils/_gcd_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t coprime_pairs = 0;

    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t lim = n / i;
        for (idx_t j = 1; j <= lim; ++j)
        {
            if (_gcd_idx(i, j) == 1)
                ++coprime_pairs;
        }
    }

    return 4 * coprime_pairs + 5;
}
