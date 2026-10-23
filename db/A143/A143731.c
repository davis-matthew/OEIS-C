/*
 * A143731  Characteristic function of numbers with at least two distinct
 *          prime factors (A024619).
 * https://oeis.org/A143731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 iff n has at least two distinct prime factors,
 *            otherwise 0
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
    if (n < 1)         /* n == 0 not part of the sequence; return 0 safely */
        return (seq_t)0;

    uint64_t t = (uint64_t)n;     /* idx_t is uint64_t */
    int count = 0;

    /* count factor 2 */
    if ((t & 1ull) == 0) {
        count++;
        while ((t & 1ull) == 0)
            t >>= 1;
    }

    /* count odd factors up to sqrt(t) */
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            count++;
            while (t % p == 0)
                t /= p;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (t > 1)
        count++;

    return (seq_t)(count >= 2 ? 1 : 0);
}
