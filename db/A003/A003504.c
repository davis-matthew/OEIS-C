/*
 * A003504  a(0)=a(1)=1; thereafter a(n+1) = (1/n)*Sum_{k=0..n} a(k)^2 (a(n)
 *          is not always integral!).
 * https://oeis.org/A003504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+1) = (a(n)^2 + (n-1)*a(n)) / n for n >= 2, a(0) = a(1) = 1,
 *            a(2) = 2 (evaluated as (a/g) * ((a+n-1)/(n/g)) with g =
 *            gcd(a(n), n) so that intermediate values stay small)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   a003504_ugcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a003504_ugcd.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    seq_t v = 2;
    for (idx_t k = 2; k < n; k++) {
        /* v = a(k); compute a(k+1) = v*(v+k-1)/k */
        uint64_t r = (uint64_t)(unsigned long long)(v % (seq_t)k);
        uint64_t g = a003504_ugcd((uint64_t)k, r);
        seq_t q = (seq_t)(k / g);
        seq_t left = v / (seq_t)g;
        seq_t right = (v + (seq_t)(k - 1)) / q;
        v = left * right;
    }
    return v;
}
