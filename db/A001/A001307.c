/*
 * A001307  Expansion of 1/(1-x)^2/(1-x^2)/(1-x^4)/(1-x^10)/(1-x^20).
 * https://oeis.org/A001307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/((1-x)^2 (1-x^2) (1-x^4) (1-x^10) (1-x^20)). a(n) =
 *            Sum_{i,j>=0, 20i+10j<=n} g(n-20i-10j), where g(m) = [x^m]
 *            1/((1-x)^2(1-x^2)(1-x^4)) = Sum_{c=0..floor(m/4)}
 *            (k-2c+1)(k-2c+1+r), with m = 2k+r, evaluated in closed form
 * Range:     u8 n <= 18, u16 n <= 86, u32 n <= 943, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a001307_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001307_g.h"

seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t i = 0; 20 * i <= n; i++) {
        idx_t rem = n - 20 * i;
        for (idx_t j = 0; 10 * j <= rem; j++) {
            res += (seq_t)a001307_g(rem - 10 * j);
        }
    }
    return res;
}
