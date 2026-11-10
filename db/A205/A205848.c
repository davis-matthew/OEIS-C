/*
 * A205848  The least number j such that 4 divides s(k)-s(j), where
 *          k(n)=A205847(n) and s(k) denotes the (k+1)-st Fibonacci number.
 * https://oeis.org/A205848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for each k>=1 in order, list every j<k (ascending) with s(k) ≡
 *            s(j) (mod 4), where s(i)=Fibonacci(i+1); a(n) is the n-th j
 *            listed (search by enumeration)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned A205848_res(uint64_t i)
{
    /* Fibonacci(i+1) mod 4, period 6 in i: i mod 6 = 0..5 */
    static const unsigned t[6] = {1, 1, 2, 3, 1, 0};
    return t[i % 6];
}

seq_t a(idx_t n)
{
    uint64_t rem = (uint64_t)n;
    if (rem == 0) return (seq_t)0;
    for (uint64_t k = 1;; ++k) {
        unsigned rk = A205848_res(k);
        uint64_t cnt = 0;
        for (uint64_t j = 1; j < k; ++j)
            if (A205848_res(j) == rk) ++cnt;
        if (rem <= cnt) {
            uint64_t seen = 0;
            for (uint64_t j = 1; j < k; ++j) {
                if (A205848_res(j) == rk) {
                    ++seen;
                    if (seen == rem) return (seq_t)j;
                }
            }
        }
        rem -= cnt;
    }
}
