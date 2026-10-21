/*
 * A070086  Areas of integer triangles [A070080(n), A070081(n), A070082(n)],
 *          rounded values.
 * https://oeis.org/A070086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = rint( sqrt( s*(s-u)*(s-v)*(s-w) ) ) where u,v,w are the
 *            side lengths of the nth integer triangle enumerated by
 *            increasing perimeter and then by smallest side, and s =
 *            (u+v+w)/2. The computation uses only integer arithmetic
 * Range:     u8 n <= 972, u16 n <= 972, u32 n <= 972, u64 n <= 972,
 *            u128 n <= 972, big any n
 * Helpers:   u128_sqrt, upow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_sqrt.h"
#include "../utils/upow2.h"

/* Main function for sequence A070086 */
seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t p = 3; ; ++p) {
        idx_t max_a = p / 3;
        for (idx_t a = 1; a <= max_a; ++a) {
            idx_t max_b = (p - a) / 2;
            for (idx_t b = a; b <= max_b; ++b) {
                idx_t c = p - a - b;
                if (a + b > c) {            /* valid triangle */
                    ++count;
                    if (count == n) {
                        seq_t u = a, v = b, w = c;
                        seq_t s2 = u + v + w;           /* 2*s */
                        seq_t t1 = s2;
                        seq_t t2 = s2 - (seq_t)2 * u;
                        seq_t t3 = s2 - (seq_t)2 * v;
                        seq_t t4 = s2 - (seq_t)2 * w;

                        unsigned __int128 numer =
                            (unsigned __int128)t1 *
                            (unsigned __int128)t2 *
                            (unsigned __int128)t3 *
                            (unsigned __int128)t4;

                        unsigned long long sqrt_numer = u128_sqrt(numer);
                        seq_t result = (seq_t)((sqrt_numer + 2ULL) / 4ULL);
                        return result;
                    }
                }
            }
        }
    }
}
