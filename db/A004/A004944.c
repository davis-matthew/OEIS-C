/*
 * A004944  a(n) = round(n*phi^9), where phi is the golden ratio, A001622.
 * https://oeis.org/A004944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^9) = 38*n + round(17*sqrt(5)*n) = 38*n +
 *            round(sqrt(1445*n^2)), since phi^9 = 38 + 17*sqrt(5)
 * Range:     u8 n <= 3, u16 n <= 862, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 N, unsigned __int128 start) {
    /* Newton's method from an upper bound: floor(sqrt(N)) */
    unsigned __int128 x = start, y;
    if (N == 0 || x == 0) return 0;
    y = (x + N / x) / 2;
    while (y < x) {
        x = y;
        y = (x + N / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 N, m;
    if (n == 0) return (seq_t)0;
    N = 1445 * nn * nn;
    m = isqrt_u128(N, 39 * nn + 1);
    /* round(sqrt(N)) = m + 1 iff N >= m^2 + m + 1 */
    if (N >= m * m + m + 1) m++;
    return u128_to_seq(38 * nn + m);
}
