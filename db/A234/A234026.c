/*
 * A234026  Permutation of nonnegative integers: a(n) = A193231(A054429(n)).
 * https://oeis.org/A234026
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A193231(A054429(n)); A054429 reverses each block [2^k,
 *            2^(k+1)-1], A193231 XORs (x+1)^e over the set bits e of n
 *            (polynomials over GF(2)). a(0) = 0
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 83 terms through n = 8014 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A054429_a(idx_t n)
{
    if (n == 0) return 0;
    unsigned k = 63 - __builtin_clzll((unsigned long long)n);
    idx_t start = ((idx_t)1) << k;
    idx_t last = (start << 1) - 1;   /* wraps correctly for k = 63 */
    return last - (n - start);
}

static seq_t A193231_a(idx_t n)
{
    seq_t res = 0;
    seq_t mask = 1;
    idx_t t = n;
    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    return A193231_a(A054429_a(n));
}
