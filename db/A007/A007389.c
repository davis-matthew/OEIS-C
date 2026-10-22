/*
 * A007389  7th-order maximal independent sets in cycle graph.
 * https://oeis.org/A007389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-9) for n >= 18, with a(1) = 0 and, for 2 <=
 *            n <= 17, a(n) = 2 for even n and a(n) = n for odd n
 * Range:     u8 n <= 36, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t buf[9];
    idx_t i;
    if (n == 1) return 0;
    if (n <= 17) {
        if (n % 2 == 0) return (seq_t)2;
        return (seq_t)n;
    }
    /* buf[i % 9] holds a(i) for the last 9 indices */
    for (i = 9; i <= 17; i++) {
        if (i % 2 == 0) buf[i % 9] = (seq_t)2;
        else buf[i % 9] = (seq_t)i;
    }
    for (i = 18; i <= n; i++) {
        /* a(i) = a(i-2) + a(i-9); a(i-9) is stored in slot i % 9 */
        buf[i % 9] += buf[(i - 2) % 9];
    }
    return buf[n % 9];
}
