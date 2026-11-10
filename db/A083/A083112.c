/*
 * A083112  Product of terms in row n of A083110.
 * https://oeis.org/A083112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of row n of A083110; row n = first n-1 integers
 *            coprime to n, then the least larger integer making the product
 *            == -1 (mod n); a(1)=1. Computed by direct search
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A083112_gcd(uint64_t x, uint64_t y)
{
    while (y != 0) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    uint64_t k = (uint64_t)n;
    seq_t P = 1;
    uint64_t pm = 1 % k;
    uint64_t last = 0;
    uint64_t count = 0;
    uint64_t cand = 1;
    while (count < k - 1) {
        if (A083112_gcd(cand, k) == 1) {
            P = P * (seq_t)cand;
            pm = (pm * (cand % k)) % k;
            last = cand;
            ++count;
        }
        ++cand;
    }
    /* modular inverse of pm mod k */
    uint64_t inv = 0;
    for (uint64_t x = 1; x < k; ++x) {
        if ((pm * x) % k == 1) { inv = x; break; }
    }
    uint64_t target = ((k - 1) * inv) % k;
    uint64_t t = 0;
    if (last + 1 > target) {
        uint64_t diff = last + 1 - target;
        t = (diff + k - 1) / k;
    }
    uint64_t lastv = target + t * k;
    P = P * (seq_t)lastv;
    return P;
}
