/*
 * A214720  Least m>0 such that n^2-m and n-m are relatively prime.
 * https://oeis.org/A214720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that gcd(n^2 - m, n - m) = 1
 * Range:     u8 n <= 341, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* Special case n == 2 */
    if (n == 2)
        return (seq_t)1;

    seq_t N  = (seq_t)n;
    seq_t N1 = N - (seq_t)1;

    /* Search for smallest m>0 satisfying gcd(N, m) == 1 &&
       gcd(N1, m-1) == 1   */
    for (idx_t m = 1; ; ++m) {
        seq_t M  = (seq_t)m;
        if (gcd(N, M) == (seq_t)1) {
            seq_t M1 = M - (seq_t)1;
            if (gcd(N1, M1) == (seq_t)1)
                return M;
        }
    }
}
