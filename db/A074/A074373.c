/*
 * A074373  Square of the sum of the prime factors of n (with repetition).
 * https://oeis.org/A074373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (sum of the prime factors of n, counted with
 *            multiplicity)^2 (a(1) = 0)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1)
        return 0;

    seq_t sum = 0;            /* sum of prime factors (with multiplicity) */
    idx_t m = n;
    /* factor out 2 */
    while ((m & 1) == 0) {
        sum += 2;
        m >>= 1;               /* m /= 2 */
    }
    /* factor out odd divisors */
    for (idx_t d = 3; d * d <= m; d += 2) {
        while (m % d == 0) {
            sum += d;
            m /= d;
        }
    }
    /* if m > 1, it is a remaining prime factor */
    if (m > 1)
        sum += (seq_t)m;

    return sum * sum;
}
