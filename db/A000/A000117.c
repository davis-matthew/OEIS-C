/*
 * A000117  Number of even sequences with period 2n (bisection of A000011).
 * https://oeis.org/A000117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = A000011(2n) = 2^(n-1) + (1/(8n)) * Sum_{d|2n}
 *            phi(2d)*2^(2n/d) for n >= 1, a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 482 (2026-11-01)
 * Helpers:   totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/totient_u64.h"

/* floor(2^k / m) as seq_t (exact modulo the width of seq_t), remainder in *rem */
static seq_t a117_pow2_div(uint64_t k, uint64_t m, uint64_t *rem)
{
    seq_t q = 0;
    uint64_t r = 1 % m;
    if (m == 1) q = 1;
    while (k >= 8) {
        r <<= 8;
        q = q * 256u + (seq_t)(r / m);
        r %= m;
        k -= 8;
    }
    while (k > 0) {
        r <<= 1;
        q = q * 2u + (seq_t)(r / m);
        r %= m;
        k--;
    }
    *rem = r;
    return q;
}

seq_t a(idx_t n)
{
    uint64_t N, m, d, rs = 0;
    seq_t tot = 0, p = 1;
    idx_t i;
    if (n == 0) return 1;
    N = 2 * (uint64_t)n;
    m = 8 * (uint64_t)n;
    for (d = 1; d <= N; d++) {
        if (N % d == 0) {
            uint64_t ph = totient_u64(2 * d);
            uint64_t r, rr;
            seq_t q = a117_pow2_div(N / d, m, &r);
            tot += q * 1u * (idx_t)ph;
            rr = ph * r;
            tot += (seq_t)(rr / m);
            rs += rr % m;
        }
    }
    tot += (seq_t)(rs / m);
    for (i = 1; i < n; i++) p = p * 2u;
    tot += p;
    return tot;
}
