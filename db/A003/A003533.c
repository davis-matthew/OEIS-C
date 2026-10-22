/*
 * A003533  Divisors of 2^25 - 1.
 * https://oeis.org/A003533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^25 - 1 = 31*601*1801 (divisors
 *            found by trial division up to sqrt)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static uint64_t dv[64];
    static int cnt = 0;
    if (cnt == 0) {
        uint64_t N = ((uint64_t)1 << 25) - 1;
        uint64_t lo[32], hi[32];
        int nl = 0, nh = 0;
        uint64_t d;
        int i;
        for (d = 1; d * d <= N; d++) {
            if (N % d == 0) {
                lo[nl++] = d;
                if (d * d != N) hi[nh++] = N / d;
            }
        }
        for (i = 0; i < nl; i++) dv[cnt++] = lo[i];
        for (i = nh - 1; i >= 0; i--) dv[cnt++] = hi[i];
    }
    if (n < 1 || n > (idx_t)cnt) return (seq_t)0;
    {
        uint64_t x = dv[n - 1];
        seq_t r = 0;
        int b;
        for (b = 31; b >= 0; b--) {
            r = r * 2 + (seq_t)((x >> b) & 1);
        }
        return r;
    }
}
