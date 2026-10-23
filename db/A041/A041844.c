/*
 * A041844  Numerators of continued fraction convergents to sqrt(444).
 * https://oeis.org/A041844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 590*a(n-2)-a(n-4), a(0) = 21, a(1) = 295, a(2) = 12411,
 *            a(3) = 174049
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)21;
    if (n == 1) return (seq_t)295;
    if (n == 2) return (seq_t)12411;
    if (n == 3) return (seq_t)174049;

    /* Even indices: a(0), a(2), a(4), … using the recurrence. */
    if ((n & 1) == 0) {
        seq_t prev2 = (seq_t)21;      /* a(0) */
        seq_t prev0 = (seq_t)12411;   /* a(2) */
        idx_t steps = (n / 2) - 1;     /* number of recurrence steps needed */
        for (idx_t i = 0; i < steps; ++i) {
            seq_t cur = 590 * prev0 - prev2;
            prev2 = prev0;
            prev0 = cur;
        }
        return prev0;
    }

    /* Odd indices: a(1), a(3), a(5), … using the recurrence. */
    {
        seq_t prev2 = (seq_t)295;      /* a(1)  */
        seq_t prev0 = (seq_t)174049;   /* a(3)  */
        idx_t steps = ((n - 1) / 2) - 1;
        for (idx_t i = 0; i < steps; ++i) {
            seq_t cur = 590 * prev0 - prev2;
            prev2 = prev0;
            prev0 = cur;
        }
        return prev0;
    }
}
