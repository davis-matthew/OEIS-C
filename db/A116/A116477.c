/*
 * A116477  a(n) = Sum_{1<=k<=n, gcd(k,n)=1} floor(n/k).
 * https://oeis.org/A116477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{1<=k<=n, gcd(k,n)=1} floor(n/k)
 * Range:     u8 n <= 60, u16 n <= 7252, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local gcd for idx_t (uint64_t) to avoid dependency on external helpers */
static inline idx_t my_gcd(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t k = 1; k <= n; ++k) {
        if (my_gcd(k, n) == 1) {
            res += (seq_t)(n / k);  /* floor division */
        }
    }
    return res;
}
