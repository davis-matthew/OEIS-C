/*
 * A003529  Divisors of 2^20 - 1.
 * https://oeis.org/A003529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^20 - 1 = 1048575 (found by
 *            trial division up to sqrt)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static uint64_t divs[64];
    static int count = 0;
    static int computed = 0;

    if (!computed) {
        uint64_t N = (1ULL << 20) - 1;
        uint64_t small[64], large[64];
        int ns = 0, nl = 0, i;
        uint64_t d;
        for (d = 1; d * d <= N; d++) {
            if (N % d == 0) {
                small[ns++] = d;
                if (d * d != N) large[nl++] = N / d;
            }
        }
        for (i = 0; i < ns; i++) divs[count++] = small[i];
        for (i = nl - 1; i >= 0; i--) divs[count++] = large[i];
        computed = 1;
    }

    if (n < 1 || n > (idx_t)count) return (seq_t)0;
    return (seq_t)divs[n - 1];
}
