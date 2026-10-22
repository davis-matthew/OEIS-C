/*
 * A006424  Number of rooted toroidal maps with 4 faces and n vertices and
 *          without separating cycles or isthmuses.
 * https://oeis.org/A006424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(136*n^7 + 6554*n^6 + 98165*n^5
 *            + 516860*n^4 + 763064*n^3 + 306776*n^2 + 2220955*n -
 *            586110)/19958400 computed as C' * (P / d), where C =
 *            binomial(n+4,5), g = gcd(C, 166320), C' = C/g, d = 166320/g (d
 *            divides P)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 13, u64 n <= 104, u128 n <= 1000,
 *            big any n
 * Helpers:   a006424_g128, gcd_seq, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/gcd_seq.h"
#include "../utils/a006424_g128.h"

seq_t a(idx_t n) {
    if (n <= 2000) {
        unsigned __int128 m = n;
        unsigned __int128 c = m * (m + 1) * (m + 2) * (m + 3) * (m + 4) / 120;
        unsigned __int128 p = ((((((136 * m + 6554) * m + 98165) * m + 516860) * m
                                + 763064) * m + 306776) * m + 2220955) * m - 586110;
        unsigned __int128 g = a006424_g128(c, 166320);
        unsigned __int128 d = 166320 / g;
        c /= g;
        return u128_to_seq(c * (p / d));
    } else {
        seq_t m = (seq_t)n;
        seq_t c = m * (m + 1) * (m + 2) * (m + 3) * (m + 4) / 120;
        seq_t p = ((((((m * 136 + 6554) * m + 98165) * m + 516860) * m
                     + 763064) * m + 306776) * m + 2220955) * m - 586110;
        seq_t g = gcd_seq(c, (seq_t)166320);
        seq_t d = (seq_t)166320 / g;
        c /= g;
        return c * (p / d);
    }
}
