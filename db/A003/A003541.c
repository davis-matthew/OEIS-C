/*
 * A003541  Divisors of 2^34 - 1.
 * https://oeis.org/A003541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   divisors of 2^34 - 1 in increasing order; found by trial
 *            division up to sqrt(2^34 - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static uint64_t divs[64];
    static int cnt = 0;
    if (cnt == 0) {
        uint64_t N = (((uint64_t)1) << 34) - 1;
        uint64_t small[32], large[32];
        int ns = 0, nl = 0;
        uint64_t d;
        for (d = 1; d * d <= N; d++) {
            if (N % d == 0) {
                small[ns++] = d;
                if (d * d != N) large[nl++] = N / d;
            }
        }
        int k = 0, i;
        for (i = 0; i < ns; i++) divs[k++] = small[i];
        for (i = nl - 1; i >= 0; i--) divs[k++] = large[i];
        cnt = k;
    }
    if (n < 1 || n > (idx_t)cnt) return (seq_t)0;
    return (seq_t)divs[n - 1];
}
