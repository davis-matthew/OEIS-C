/*
 * A005873  Theta series of hexagonal close-packing with respect to
 *          tetrahedral hole.
 * https://oeis.org/A005873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   count hcp points at squared distance n/24 from a tetrahedral
 *            hole (unit spacing, layer height h = sqrt(2/3), hole at height
 *            h/4 above an A layer). a(n) = Sum_{k : (4k-1)^2 <= n} [k odd]
 *            rB((n-(4k-1)^2)/24) + [k even] rA((n-(4k-1)^2)/8), rB(m) =
 *            #{(i,j): i^2+ij+j^2 = m}, rA(m) = #{(x,y): x == y ==...
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   a005873_isqrt, a005873_mod3, a005873_rA, a005873_rB
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a005873_isqrt.h"
#include "../utils/a005873_mod3.h"
#include "../utils/a005873_rA.h"
#include "../utils/a005873_rB.h"

seq_t a(idx_t n) {
    long long N = (long long)n;
    long long S = a005873_isqrt(N);
    long long total = 0;
    long long K = S / 4 + 2;
    for (long long k = -K; k <= K; k++) {
        long long t = 4 * k - 1;
        long long v = t * t;
        if (v > N) continue;
        long long rem = N - v;
        if ((k % 2) != 0) {
            if (rem % 24 == 0) total += a005873_rB(rem / 24);
        } else {
            if (rem % 8 == 0) total += a005873_rA(rem / 8);
        }
    }
    return (seq_t)total;
}
