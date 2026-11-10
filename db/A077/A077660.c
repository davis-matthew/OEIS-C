/*
 * A077660  Sum of terms of n-th row of A077583.
 * https://oeis.org/A077660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of row n of A077583: the first n integers k >= n with
 *            gcd(k,n) > 1 (a(1) = 1); computed by a direct scan, with a
 *            closed form n*n*(n+1)/2 when n is prime
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A077660_gcd(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static bool A077660_isprime(idx_t n) {
    if (n < 2) return false;
    for (idx_t d = 2; d * d <= n; ++d)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (A077660_isprime(n)) {
        seq_t s = (seq_t)n * ((seq_t)n + 1) / 2;
        return s * (seq_t)n;
    }
    seq_t sum = 0;
    idx_t count = 0;
    for (idx_t k = n; count < n; ++k) {
        if (A077660_gcd(k, n) > 1) {
            sum += (seq_t)k;
            ++count;
        }
    }
    return sum;
}
