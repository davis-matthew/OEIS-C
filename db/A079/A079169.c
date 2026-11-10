/*
 * A079169  Difference between A079168(n) and A079167(n).
 * https://oeis.org/A079169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i>=1} (i^2 - i)*e_i, where n = p_1^e_1*...*p_k^e_k
 *            with p_1<...<p_k (A079168(n) - A079167(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 122 terms through n = 9711 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    seq_t sum = 0;
    uint64_t idx = 1;
    uint64_t p = 2;

    while (nn > 1 && p <= nn / p) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                e++;
            }
            sum += (seq_t)(idx * (idx - 1)) * (seq_t)e;
            idx++;
        }
        p += (p == 2) ? 1 : 2;
    }
    if (nn > 1) {
        sum += (seq_t)(idx * (idx - 1));
    }
    return sum;
}
