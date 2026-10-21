/*
 * A083399  Number of divisors of n that are not divisors of other divisors of
 *          n.
 * https://oeis.org/A083399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = omega(n) + 1 (number of distinct prime factors + 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1) {                /* a(1) = 1, and for n=0 (unused) return 1 */
        return (seq_t)1;
    }

    idx_t m = n;
    uint64_t cnt = 0;            /* number of distinct prime factors */

    /* factor out 2 */
    if ((m & 1) == 0) {
        cnt++;
        while ((m & 1) == 0) m >>= 1;
    }

    /* odd factors */
    for (idx_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0) {
            cnt++;
            while (m % d == 0) m /= d;
        }
    }

    /* remaining prime > 2 */
    if (m > 1) cnt++;

    return (seq_t)cnt + (seq_t)1;
}
