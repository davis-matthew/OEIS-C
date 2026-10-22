/*
 * A001392  a(n) = 9*binomial(2n,n-4)/(n+5).
 * https://oeis.org/A001392
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(2*n-1)*a(n-1)/((n+5)*(n-4)), a(4) = 1 [equivalently
 *            a(n) = 9*binomial(2n,n-4)/(n+5)]
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   a001392_gcd64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001392_gcd64.h"

seq_t a(idx_t n) {
    static idx_t cur_n = 0;
    static seq_t cur_v;
    if (n < 4) return 0;
    if (cur_n == 0 || n < cur_n) {
        cur_n = 4;
        cur_v = 1;
    }
    while (cur_n < n) {
        idx_t k = cur_n + 1;
        uint64_t num = (uint64_t)(2 * k) * (uint64_t)(2 * k - 1);
        uint64_t den = (uint64_t)(k + 5) * (uint64_t)(k - 4);
        uint64_t g = a001392_gcd64(num, den);
        num /= g;
        den /= g;
        /* den is now coprime to num, so it divides cur_v exactly */
        cur_v /= den;
        cur_v *= num;
        cur_n = k;
    }
    return cur_v;
}
