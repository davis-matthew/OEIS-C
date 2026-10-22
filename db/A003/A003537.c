/*
 * A003537  Divisors of 2^29 - 1.
 * https://oeis.org/A003537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = n-th smallest divisor of 2^29 - 1 (found by trial
 *            division up to sqrt)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t N = (((uint64_t)1) << 29) - 1;
    uint64_t small[64], large[64];
    int ns = 0, nl = 0;
    uint64_t i;
    for (i = 1; i * i <= N; i++) {
        if (N % i == 0) {
            small[ns++] = i;
            if (i * i != N) large[nl++] = N / i;
        }
    }
    /* small is increasing, large is decreasing */
    idx_t total = (idx_t)(ns + nl);
    if (n < 1 || n > total) return (seq_t)0;
    uint64_t v;
    if (n <= (idx_t)ns) {
        v = small[n - 1];
    } else {
        idx_t k = n - 1 - (idx_t)ns;
        v = large[(idx_t)nl - 1 - k];
    }
    return (seq_t)v;
}
