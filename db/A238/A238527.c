/*
 * A238527  Prime values of A238525.
 * https://oeis.org/A238527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th term (in order, with repeats) of A238525 that is
 *            prime, where A238525(m) = m mod sopfr(m); computed by scanning m
 *            = 2, 3,
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 8862 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A238527_sopfr_mod(idx_t n)
{
    idx_t m = n, sum = 0;
    while ((m & 1ULL) == 0) {
        sum += 2;
        m >>= 1;
    }
    for (idx_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            sum += p;
            m /= p;
        }
    }
    if (m > 1)
        sum += m;
    return n % sum;
}

static bool A238527_isprime(idx_t v)
{
    if (v < 2)
        return false;
    if (v < 4)
        return true;
    if ((v & 1ULL) == 0)
        return false;
    for (idx_t p = 3; p <= v / p; p += 2)
        if (v % p == 0)
            return false;
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 2;; m++) {
        idx_t v = A238527_sopfr_mod(m);
        if (A238527_isprime(v)) {
            count++;
            if (count == n)
                return (seq_t)v;
        }
    }
}
