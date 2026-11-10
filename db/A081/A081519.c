/*
 * A081519  Sum of terms in row n of A081520.
 * https://oeis.org/A081519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum of the n-1 smallest integers m >= 2 with gcd(n,m)
 *            > 1 (row sum of A081520; computed by direct scan)
 * Range:     u8 n <= 10, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 997 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A081519_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t sum = 1;
    idx_t count = 0, m = 2;
    if (n < 2)
        return sum;
    while (count < n - 1) {
        if (A081519_gcd(n, m) > 1) {
            sum += (seq_t)m;
            count++;
        }
        ++m;
    }
    return sum;
}
