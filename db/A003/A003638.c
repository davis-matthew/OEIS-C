/*
 * A003638  Number of classes per genus in quadratic field with discriminant
 *          -4n, -n == 2,3( mod 4).
 * https://oeis.org/A003638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let m(n) be the n-th squarefree m with m == 1,2 (mod 4) (so -m
 *            == 3,2 mod 4). a(n) = h(-4m) / 2^r, where h(-4m) is the class
 *            number (counted by enumerating reduced primitive forms of
 *            discriminant -4m) and r = number of odd primes dividing m (2^r =
 *            number of genera)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   classno, g3, sqfree_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/classno.h"
#include "../utils/g3.h"
#include "../utils/sqfree_ok.h"

seq_t a(idx_t n) {
    uint64_t m = 0, cnt = 0;
    while (cnt < n) {
        m++;
        if (sqfree_ok(m)) cnt++;
    }
    uint64_t h = classno(m);
    uint64_t t = m;
    while (t % 2 == 0) t /= 2;
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            h /= 2;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) h /= 2;
    return (seq_t)h;
}
