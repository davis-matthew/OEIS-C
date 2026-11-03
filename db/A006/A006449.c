/*
 * A006449  Row sums of Fibonacci-Pascal triangle in A045995.
 * https://oeis.org/A006449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} F(binomial(n,k)), where F = A000045
 *            (Fibonacci numbers)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 15 terms through n = 14 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t fib_u64(uint64_t m) {
    seq_t x = 0, y = 1; /* F(k), F(k+1) */
    int bit = 63;
    while (bit >= 0 && !((m >> bit) & 1)) bit--;
    for (; bit >= 0; bit--) {
        seq_t c = x * (y + y - x);
        seq_t d = x * x + y * y;
        if ((m >> bit) & 1) {
            x = d;
            y = c + d;
        } else {
            x = c;
            y = d;
        }
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t result = 0;
    unsigned __int128 c = 1;
    idx_t k;
    for (k = 0; k <= n / 2; k++) {
        seq_t f = fib_u64((uint64_t)c);
        if (k + k == n)
            result += f;
        else
            result += f + f;
        c = c * (unsigned __int128)(n - k) / (unsigned __int128)(k + 1);
    }
    return result;
}
