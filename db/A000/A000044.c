/*
 * A000044  Dying rabbits: a(0) = 1; for 1 <= n <= 12, a(n) = Fibonacci(n);
 *          for n >= 13, a(n) = a(n-1) + a(n-2) - a(n-13).
 * https://oeis.org/A000044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-7) + a(n-9) + a(n-11),
 *            a(0) = 1, a(k) = 0 for k < 0 (from G.f. 1/(1 - z - z^3 - z^5 -
 *            z^7 - z^9 - z^11))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t w[12];
    idx_t i;
    int k, j;
    for (k = 0; k < 12; k++)
        w[k] = 0;
    w[0] = 1;
    for (i = 1; i <= n; i++) {
        seq_t s = 0;
        for (j = 1; j <= 11; j += 2) {
            if (i >= (idx_t)j)
                s += w[(i - (idx_t)j) % 12];
        }
        w[i % 12] = s;
    }
    return w[n % 12];
}
