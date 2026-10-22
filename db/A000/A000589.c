/*
 * A000589  a(n) = 11*binomial(2n,n-5)/(n+6).
 * https://oeis.org/A000589
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+6)*(n-5)*a(n) = 2*n*(2*n-1)*a(n-1), a(5) = 1 [a(n) =
 *            11*binomial(2n,n-5)/(n+6)]
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   g589_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/g589_gcd.h"

seq_t a(idx_t n) {
    seq_t x = 1;
    idx_t i;
    for (i = 6; i <= n; i++) {
        uint64_t P = (uint64_t)(2 * i) * (uint64_t)(2 * i - 1);
        uint64_t Q = (uint64_t)(i + 6) * (uint64_t)(i - 5);
        uint64_t g = g589_gcd(P, Q);
        P /= g;
        Q /= g;
        /* gcd(P,Q)=1 and x*P/Q is an integer, so Q divides x */
        x = x / (seq_t)Q * (seq_t)P;
    }
    return x;
}
