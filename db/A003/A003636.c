/*
 * A003636  Number of classes per genus in quadratic field with discriminant
 *          -n.
 * https://oeis.org/A003636
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(D)/2^(t-1), where D = -m is the (n-2)-th negative
 *            fundamental discriminant (m = 3, 4, 7, 8, 11, 15, ...), h(D) is
 *            its class number (counted by enumerating reduced forms) and t is
 *            the number of distinct primes dividing m (so 2^(t-1) is the
 *            number of genera)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   class_number, is_fund, sqfree_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/class_number.h"
#include "../utils/is_fund.h"
#include "../utils/sqfree_ll.h"

static int omega_ll(long long m) {
    int t = 0;
    long long p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            t++;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) t++;
    return t;
}

seq_t a(idx_t n) {
    long long target = (long long)n - 3;
    long long cnt = -1, m = 2;
    while (cnt < target) {
        m++;
        if (is_fund(m)) cnt++;
    }
    long long h = class_number(m);
    int t = omega_ll(m);
    return (seq_t)(h >> (t - 1));
}
