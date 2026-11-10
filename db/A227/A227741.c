/*
 * A227741  Simple self-inverse permutation of natural numbers: List each
 *          block of A005811(n) numbers from A173318(n-1)+1 to A173318(n) in
 *          reverse order.
 * https://oeis.org/A227741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = S(k) + S(k-1) + 1 - n, where S(k) = A173318(k) =
 *            Sum_{i=1..k} A005811(i) and k = A227737(n) is the least k with
 *            S(k) >= n (reverses each block of A005811(k) consecutive
 *            integers)
 * Range:     u8 n <= 253, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 85 terms through n = 991 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t prev = 0, sum = 0;
    for (idx_t k = 1; ; ++k) {
        uint64_t x = (uint64_t)k;
        uint64_t g = x ^ (x >> 1);
        idx_t cnt = (idx_t)__builtin_popcountll(g);
        prev = sum;
        sum += cnt;
        if (sum >= n) {
            return (seq_t)(prev + sum + 1 - n);
        }
    }
}
