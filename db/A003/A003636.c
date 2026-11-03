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
 * Validated: partial, pass, 102 terms through n = 104 (2026-11-01)
 * Helpers:   is_squarefree_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree_i64.h"

static long long class_number(long long m) {
    long long h = 0, a, b;
    for (a = 1; 3 * a * a <= m; a++) {
        for (b = -a + 1; b <= a; b++) {
            long long num = b * b + m;
            if (num % (4 * a) != 0) continue;
            long long c = num / (4 * a);
            if (c < a) continue;
            if (b < 0 && a == c) continue;
            h++;
        }
    }
    return h;
}

static bool is_fund(long long m) {
    long long r = m % 4;
    if (r == 3) return is_squarefree_i64(m);
    if (r == 0) {
        long long k = m / 4, s = k % 4;
        if (s == 1 || s == 2) return is_squarefree_i64(k);
    }
    return false;
}

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
