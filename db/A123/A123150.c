/*
 * A123150  a(n) = (n^5 - n^4 - n^3 + n^2 - 1)*a(n-5) for n > 4, otherwise n!.
 * https://oeis.org/A123150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! for n < 5, otherwise a(n) = (n^5 - n^4 - n^3 + n^2 -
 *            1)*a(n-5)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For small n, return n! directly. */
    if (n < 5ULL) {
        seq_t res = 1;
        for (idx_t i = 2; i <= n; ++i)
            res *= (seq_t)i;
        return res;
    }

    /* last[i] holds a(i) for the five most recent indices. */
    seq_t last[5];

    /* Initialize the first five terms: factorials. */
    last[0] = 1;                      /* a(0) */
    for (idx_t i = 1; i <= 4; ++i)
        last[i % 5] = last[(i - 1) % 5] * (seq_t)i;

    /* Iterate from 5 up to n using the recurrence. */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t ic  = (seq_t)i;
        seq_t ic2 = ic * ic;           /* n^2 */
        seq_t ic3 = ic2 * ic;          /* n^3 */
        seq_t ic4 = ic3 * ic;          /* n^4 */
        seq_t ic5 = ic4 * ic;          /* n^5 */
        seq_t factor = ic5 - ic4 - ic3 + ic2 - 1;
        last[i % 5] = factor * last[(i - 5) % 5];
    }

    return last[n % 5];
}
